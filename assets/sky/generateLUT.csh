#version 460 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (binding = 0) uniform writeonly image3D lut;
layout (binding = 1) uniform sampler2D transmittanceLUT;

uniform vec3 lutSize;

const float PI = 3.14159265359;


const float Rp = 6360000.0;
const float Ra = 6420000.0;

const float g = -0.8;


const float rayleighDensity = 8000.0;
const vec3 rayleighCoeff = vec3(5.47e-6, 1.28e-5, 3.12e-5);

const float mieDensity = 1200.0;
const float mieCoeff = 2e-6;

// Sun angular radius (in radians)
const float sunRadius = 0.1;

const vec3 solarIrradiance = vec3(1000.0, 1.0, 0.9);


float safeSqrt(float a)
{
    return sqrt(max(a, 0.0));
}


// Schlick approximation of Henyey-Greenstein phase function
//	cosTheta: angle between light direction + out scatter direction
//	g: strength of forward-backward scatter (-1, 1)
float phaseMie(float cosTheta)
{
    float k = 1.55 * g - 0.55 * g * g * g;

    float term = 1 + k * cosTheta;

    return (1.0 - k * k) /
    (4 * PI * term * term);
}

float phaseRayleigh(float cosTheta)
{
    return 0.75 * (1 + cosTheta * cosTheta);
}

// Adjusted F_R(theta) from Elek (2009)
float phaseRayleigh2(float cosTheta)
{
    return 0.8 * (1.4 + 0.5 * cosTheta);
}

//	h: sample altitude
//	H: scattering scale height (HR ~8000m, HM ~1200m)
float particleDensity(float h, float H)
{
    return exp(-h / H);
}


float unitRangeToUV(float x, float pixs)
{
    return 0.5 / pixs + x * (1.0 - 1.0 / pixs);
}

float clampAtmos(float a)
{
    return clamp(a, Rp, Ra);
}


float distToEdgeOfAtmos(float r, float mu)
{
    float disc = r * r * (mu * mu - 1.0) + Ra * Ra;
    return max(-r * mu + safeSqrt(disc), 0.0);
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
        * smoothstep(-sinThetaH * sunRadius, sinThetaH * sunRadius, muS - cosThetaH);
}

// Single-scattering intensity
void singleScatteringSample(float r, float mu, float muS, float nu, float d, bool rayHitsGround,
                            out vec3 rayleigh, out vec3 mie)
{
    float rQ = clampAtmos(sqrt(d * d + 2.0 * r * mu * d + r * r));
    float muSQ = clamp((r * muS + d * nu) / rQ, -1.0, 1.0);

    vec3 transmittance = calcTransmittance(r, mu, d, rayHitsGround)
        * sunTransmittance(rQ, muSQ);

    rayleigh = transmittance * particleDensity(rQ - Rp, rayleighDensity);
    mie = transmittance * particleDensity(rQ - Rp, mieDensity);
}

void singleScattering(float r, float mu, float muS, float nu, bool rayHitsGround,
                      out vec3 rayleigh, out vec3 mie)
{
    const int samples = 50;

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
    rayleigh = rayleighSum * dx * solarIrradiance * rayleighCoeff;
    mie = mieSum * dx * solarIrradiance * mieCoeff;
    rayleigh = rayleighSum;
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
    h = sqrt(uvw.x * uvw.x * (Ra * Ra - Rp * Rp) + Rp * Rp);
    cosTheta = (2 * uvw.y - 1.0);
    cosDelta = (-(0.8 + log(uvw.z * (1.0 - exp(-3.6)))) / 2.8);
}


void main()
{
    // Compute single-scattering to generate the initial LUT 
    float h, cosTheta, cosDelta;
    UVWtoHThetaDelta(vec3(gl_GlobalInvocationID.xyz) / vec3(64, 256, 256), h, cosTheta, cosDelta);

    vec3 rayleigh, mie;
    singleScattering(h, cosTheta, cosDelta, 0.0, false, rayleigh, mie);
    singleScatteringSample(h, cosTheta, cosDelta, -1.0, distToEdgeOfAtmos(h, cosTheta), false, rayleigh, mie);
    imageStore(lut, ivec3(gl_GlobalInvocationID.xyz), vec4(rayleigh, 1.0)); 
}
