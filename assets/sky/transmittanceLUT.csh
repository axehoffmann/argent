#version 460 core

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout (binding = 2, rgba32f) uniform writeonly image2D lut;


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

const float PI = 3.14159265359;

float safeSqrt(float a)
{
return sqrt(max(a, 0.0));
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

float opticalLength(AtmosphereProfile profile, float r, float mu)
{
    const int samples = 500;

    float dx = distToEdgeOfAtmos(r, mu) / float(samples);

    float total = 0.0;
    for (int i = 0; i < samples; i++)
    {
        float di = i * dx;
        float ri = sqrt(di*di + 2.0 * r * mu * di + r*r);
        float yi = getProfileDensity(profile, ri - Rp);

        float weight = i == 0 || i == (samples - 1) ? 0.5 : 1.0;
        total += weight * yi * dx;
    }
    return total;
}

vec3 transmittance(float r, float mu)
{
    return exp(-(
        rayleighScattering * opticalLength(rayleighDensity, r, mu)
        + mieScattering * opticalLength(mieDensity, r, mu)
        + absorpExtinction * opticalLength(absorpDensity, r, mu)
    ));
}


// E Bruneton's 2017 mapping
void UVtoMuR(vec2 uv, out float mu, out float r)
{
    float H = sqrt(Ra * Ra - Rp * Rp);
    float x_mu = uv.x;
    float x_r = uv.y;
    float rho = H * x_r;
    r = sqrt(rho * rho + Rp * Rp);
    float d_min = Ra - r;
    float d_max = rho + H;
    float d = d_min + x_mu * (d_max - d_min);
    mu = d <= 0.0 ? float(1.0) : (H * H - rho * rho - d * d) / (2.0 * r * d);
    mu = clamp(mu, -1.0, 1.0);
}

void main()
{
    vec2 uv = vec2(gl_GlobalInvocationID.xy) / vec2(256, 64);
    
    float zenith, h;
    UVtoMuR(uv, zenith, h);

    vec4 o = vec4(transmittance(h, zenith), 1.0);

    imageStore(lut, ivec2(gl_GlobalInvocationID.xy), o); 
}
