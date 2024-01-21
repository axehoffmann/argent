#version 460 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (binding = 0, rgba16) uniform writeonly image3D lut;
layout (binding = 1) uniform sampler2D transmittanceLUT;

const float PI = 3.14159265359;


struct AtmosphereLayer
{
    float w;
    float exp;
    float expScale;
    float lin;
    float cons;
};

struct AtmosphereProfile
{
    AtmosphereLayer layers[2];
};

const vec3 lutSize = vec3(128, 512, 512);

const vec3 solarIrradiance = vec3(1.474000, 1.850400, 1.911980);
const float sunAngularRadius = 0.004675;
const float Rp = 6360.000000;
const float Ra = 6420.000000;
const AtmosphereProfile rayleighDensity = AtmosphereProfile(AtmosphereLayer[2](
    AtmosphereLayer(0.000000, 0.000000, 0.000000, 0.000000, 0.000000), 
    AtmosphereLayer(0.000000, 1.000000, -0.125000, 0.000000, 0.000000))
);
const vec3 rayleighScattering = vec3(0.005802, 0.013558, 0.033100);
const AtmosphereProfile mieDensity = AtmosphereProfile(AtmosphereLayer[2](
    AtmosphereLayer(0.000000, 0.000000, 0.000000, 0.000000, 0.000000), 
    AtmosphereLayer(0.000000, 1.000000, -0.833333, 0.000000, 0.000000))
);
const vec3 mieScattering = vec3(0.003996, 0.003996, 0.003996);
const vec3 mieExtinction = vec3(0.004440, 0.004440, 0.004440);
const float g = 0.800000;
const AtmosphereProfile absorpDensity = AtmosphereProfile(AtmosphereLayer[2](
    AtmosphereLayer(25.000000, 0.000000, 0.000000, 0.066667, -0.666667), 
    AtmosphereLayer(0.000000, 0.000000, 0.000000, -0.066667, 2.666667))
);
const vec3 absorpExtinction = vec3(0.000000, 0.000000, 0.000000);
const vec3 groundAlbedo = vec3(0.100000, 0.100000, 0.100000);
const float muSMin = -0.500000;

const vec3 SKY_SPECTRAL_RADIANCE_TO_LUMINANCE = vec3(114974.916437, 71305.954816, 65310.548555);
const vec3 SUN_SPECTRAL_RADIANCE_TO_LUMINANCE = vec3(98242.786222, 69954.398112, 66475.012354);


float safeSqrt(float a)
{
    return sqrt(max(a, 0.0));
}


float distToEdgeOfAtmos(float r, float mu)
{
    float disc = r * r * (mu * mu - 1.0) + Ra * Ra;
    return max(-r * mu + safeSqrt(disc), 0.0);
}

float getLayerDensity(AtmosphereLayer layer, float h)
{
    float dens = layer.exp * exp(layer.expScale * h) + layer.lin * h + layer.cons;
    return clamp(dens, 0.0, 1.0);
}

float getProfileDensity(AtmosphereProfile profile, float h)
{
    return h < profile.layers[0].w
        ? getLayerDensity(profile.layers[0], h)
        : getLayerDensity(profile.layers[1], h);
}

float unitRangeToUV(float x, float pixs)
{
    return 0.5 / pixs + x * (1.0 - 1.0 / pixs);
}

float clampAtmos(float a)
{
    return clamp(a, Rp, Ra);
}

// Calculates the distance to the planet surface, assuming (r, mu) hits the ground 
float distToSurface(float r, float mu)
{
    float disc = r * r * (mu * mu - 1.0) + Rp * Rp;
    return max(-r * mu + safeSqrt(disc), 0.0);
}

float distToBoundary(float r, float mu, bool rayHitsGround)
{
    return rayHitsGround ? distToSurface(r, mu) : distToEdgeOfAtmos(r, mu);
}


// E Bruneton's 2017 mapping
vec2 RMuToUV(float r, float mu)
{
    float H = sqrt(Ra * Ra - Rp * Rp);
    float rho = safeSqrt(r * r - Rp * Rp);
    float d = distToEdgeOfAtmos(r, mu);
    float dMin = Ra - r;
    float dMax = rho + H;
    float muX = (d - dMin) / (dMax - dMin);
    float rX = rho / H;
    return vec2(unitRangeToUV(muX, 256.0), unitRangeToUV(rX, 64.0)); 
}


// Samples transmittance to the edge of atmosphere along ray (r, mu)
vec3 sampleTransmittance(float r, float mu)
{
    return texture(transmittanceLUT, RMuToUV(r, mu)).rgb;
}


// Samples transmittance through ray (r, mu) over distance d 
vec3 calcTransmittance(float r, float mu, float d, bool rayHitsGround)
{
    // define Q as point d units away along ray (r, mu)
    // length O -> Q 
    float rQ = clampAtmos(sqrt(d * d + 2.0 * r * mu * d + r * r));
    // azimuth angle at point Q (dot(mu, |Q|))
    float muQ = clamp((r * mu + d) / rQ, -1.0, 1.0);

    if (rayHitsGround)
    {
        return min(
            sampleTransmittance(rQ, -muQ) / sampleTransmittance(r, -mu),
            vec3(1.0)
        );
    }
    
    return min(
        sampleTransmittance(r, mu) / sampleTransmittance(rQ, muQ),
        vec3(1.0)
    );
}


// Transmittance to sun - depends on how much sun is above the horizon
// r: length O -> P      muS: cosine angle between sun and vertical
vec3 sunTransmittance(float r, float muS)
{
    float sinThetaH = Rp / r;
    float cosThetaH = -sqrt(max(1.0 - sinThetaH * sinThetaH, 0.0));

    return sampleTransmittance(r, muS)
        * smoothstep(-sinThetaH * sunAngularRadius, sinThetaH * sunAngularRadius, muS - cosThetaH);
}

// Single-scattering intensity
void singleScatteringSample(float r, float mu, float muS, float nu, float d, bool rayHitsGround,
                            out vec3 rayleigh, out vec3 mie)
{
    float rQ = clampAtmos(sqrt(d * d + 2.0 * r * mu * d + r * r));
    float muSQ = clamp((r * muS + d * nu) / rQ, -1.0, 1.0);

    vec3 transmittance = calcTransmittance(r, mu, d, rayHitsGround)
        * sunTransmittance(rQ, muSQ);

    rayleigh = transmittance * getProfileDensity(rayleighDensity, rQ - Rp);
    mie = transmittance * getProfileDensity(mieDensity, rQ - Rp);    
}

void singleScattering(float r, float mu, float muS, float nu, bool rayHitsGround,
                      out vec3 rayleigh, out vec3 mie)
{
    const int samples = 500;

    // Discretely sample integral
    float dx = distToBoundary(r, mu, rayHitsGround) / float(samples);
    vec3 rayleighSum = vec3(0.0);
    vec3 mieSum = vec3(0.0);
    for (int i = 0; i < samples; i++)
    {
        float di = float(i) * dx;
        vec3 rayleighi, miei;
        singleScatteringSample(r, mu, muS, nu, di, rayHitsGround, rayleighi, miei);
        // First/last samples have less weight
        float w = (i == 0 || i == samples - 1) ? 0.5 : 1.0;
        rayleighSum += rayleighi * w;
        mieSum += miei * w;
    }
    rayleigh = rayleighSum * dx * solarIrradiance * rayleighScattering;
    mie = mieSum * dx * solarIrradiance * mieScattering;
}

// LUT dimensions:
//	h: altitude (0, H)
//	theta: view-zenith angle (0, PI)
//	delta: sun-zenith angle (0, PI)
//
// mapping to 3D tex coords:
//      U = sqrt((h * h - Rp * Rp) / (Ra * Ra - Rp * Rp))
//		where Rp is planet radius, Ra is atmosphere radius
//	V = (1 + cos(theta)) / 2
//	W = (1 - exp(-2.8 * cos(delta) - 0.8)) / (1 - exp(-3.6)
void UVWtoHThetaDelta(vec3 uvw, out float h, out float cosTheta, out float cosDelta)
{
    vec3 tc = clamp((uvw * lutSize - 0.5) / (lutSize - 1.0), 0.0, 1.0);
    
    h = max(tc.x*tc.x * (Ra - Rp), 0.0);
    
    // angle to horizon 
    float cosH = -sqrt(h * (h + 2.0 * Rp)) / (Rp + h);

    if (tc.y > 0.5)
    {
        tc.y = clamp((tc.y - (0.5 + 0.5 / lutSize.y)), 0.0, 1.0) * lutSize.y / (lutSize.y / 2.0 - 1.0);
        tc.y = pow(tc.y, 5.0);
        cosTheta = max((cosH + tc.y * (1 - cosH)), cosH + 1e-4);
    }
    else
    {
        tc.y = clamp((tc.y - 0.5 / lutSize.y), 0.0, 1.0) * lutSize.y / (lutSize.y / 2.0 - 1.0);
        tc.y = pow(tc.y, 5.0);
        cosTheta = min(cosH - tc.y * (cosH + 1), cosH - 1e-4);
    }
    cosTheta = clamp(cosTheta, -1.0, 1.0);
    
    cosDelta = tan((2.0 * tc.z - 1.0 + 0.26) * 0.75) / tan(1.26 * 0.75);
    cosDelta = clamp(cosDelta, -1.0, 1.0);
}


bool rayRMuHitsGround(float r, float mu)
{
    return mu < 0.0 && r*r * (mu*mu - 1.0) + Rp*Rp >= 0.0;
}

void main()
{
    // Compute single-scattering to generate the initial LUT 
    float h, cosTheta, cosDelta;
    UVWtoHThetaDelta(vec3(gl_GlobalInvocationID.xyz) / lutSize, h, cosTheta, cosDelta);

    vec3 rayleigh, mie;
    singleScattering(h + Rp, cosTheta, cosDelta, 0.0, rayRMuHitsGround(h + Rp, cosTheta), rayleigh, mie);
    imageStore(lut, ivec3(gl_GlobalInvocationID.xyz), vec4(rayleigh, mie.r)); 
}
