#version 460 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (binding = 0) uniform writeonly image3D lut;
layout (binding = 1) uniform sampler2D transmittanceLUT;

uniform vec3 lutSize;

const float PI = 3.14159265359;


const float Rp = 6360.0;
const float Ra = 6420.0;

const float g = -0.8;

const vec3 rayleighCoeff = vec3(5.47e-6, 1.28e-5, 3.12e-5);
const float mieCoeff	 = 2e-6;

const float sunAngularRadius = 0.1;

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

// Intersect ray with the atmosphere, returning distance to entry and exit
vec2 raySphere(vec3 P, vec3 dir)
{
    float a = 1.0;
    float b = 2 * dot(P, dir);
    float c = dot(P, P) - Ra * Ra;
    float d = b * b - 4 * a * c;

    if (d > 0) // ray intersected sphere
    {
        float s = sqrt(d);
        float near = max(0, (-b - s) / (2 * a));
        float far = (-b + s) / (2 * a);

        if (far >= 0)
        {
            return vec2(near, far - near);
        }
    }

    // no intersection
    return vec2(1e20, 0);
}

float unitRangeToUV(float x, float pixs)
{
    return 0.5 / pixs + x * (1.0 - 1.0 / pixs);
}



vec4 sampleTransmittance(vec3 pos, vec3 dir)
{
    float h = distance(vec3(0.0), pos) - Rp;

    float H = sqrt(Ra * Ra - Rp * Rp);
    // Dist to hrzn
    float rho = sqrt(h * h - Rp * Rp);

    float d = raySphere(pos, dir).y;
    float dMin = Ra - h;
    float dMax = rho + H;
    float mu = (d - dMin) / (dMax - dMin);
    float r = rho / H;
    vec2 uv = vec2(unitRangeToUV(mu, 256.0), unitRangeToUV(r, 64.0));
    return texture(transmittanceLUT, uv);
}

// Calculate the transmittance between two points in atmosphere
// via 2 samples of the transmittance LUT
vec4 calcTransmittance(vec3 Pa, vec3 Pb)
{
    vec4 tPa = sampleTransmittance(
        Pa, normalize(Pb - Pa)
    );
    vec4 tPb = sampleTransmittance(
        Pb, normalize(Pb - Pa)
    );

    return min(tPa / tPb, vec4(1.0));
}

// Transmittance to sun - depends on how much sun is above the horizon
vec4 sunTransmittance(vec3 pos, vec3 dir)
{
    float r = distance(vec3(0.0), pos);
    float mu = dot(normalize(pos), dir);

    float sinThetah = Rp / r;
    float cosThetah = -sqrt(max(1.0 - sinThetah * sinThetah, 0.0));
    // Assumes sun is point-like, yet a fraction represented by smoothstep term
    // can be above/below the horizon
    return sampleTransmittance(pos, dir) * smoothstep(
        -sinThetah * sunAngularRadius, // / rad,
        sinThetah * sunAngularRadius, // / rad,
        mu - cosThetah
    );
}

// Single-scattering intensity
vec4 intensity(float delta, vec3 Pa, vec3 Pb)
{
    vec3 incidentIntensity = vec3(1.0, 1.0, 0.9);

    vec3 coeffR = rayleighCoeff / (4 * PI);
    float coeffM = mieCoeff / (4 * PI);

    vec3 L = vec3(cos(delta), sin(delta), 0.0);
    // Discretely sample integral part
    vec3 integR = vec3(0.0);
    float integM = 0.0;

    const int samples = 50;
    for (int i = 0; i < samples; i++)
    {
        // Sample point
        vec3 P = mix(Pa, Pb, float(i) / float(samples));
        // Point towards sun at edge of atmosphere from sample
        vec3 Pc = raySphere(P, L).y * L + P;

        float pR = particleDensity(P.y, 8000.0);
        float pM = particleDensity(P.y, 1200.0);

        // transmittance of sunlight through atmosphere to P
        vec4 scatteredT = sunTransmittance(P, L);

        // manually calculate transmittance based on distance here
        // transmittance LUT doesn't accomodate between two points
        // in atmosphere
        vec4 directT = calcTransmittance(Pa, P);
        
        integR += pR * exp(-scatteredT.rgb - directT.rgb) * 1.0 / 50.0;
        integM += pM * exp(-scatteredT.a - directT.a)     * 1.0 / 50.0;
    }
    
    vec3 IsR = incidentIntensity * coeffR * integR;
    vec3 IsM = incidentIntensity * coeffM * integM;

    return vec4(IsR + IsM, 1.0);
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
void UVWtoHThetaDelta(vec3 uvw, out float h, out float theta, out float delta)
{
    h = sqrt(uvw.x * uvw.x * (Ra * Ra - Rp * Rp) + Rp * Rp);
    theta = acos(2 * uvw.y - 1.0);
    delta = acos((-1.8 + uvw.z * (1.0 - exp(-3.6))) / 2.8);
}

void main()
{
    // Compute single-scattering to generate the initial LUT
    float h, theta, delta;
    UVWtoHThetaDelta(vec3(gl_GlobalInvocationID.xyz) / vec3(64, 256, 256), h, theta, delta);

    vec3 pos = vec3(0.0, h, 0.0);
    vec3 V = vec3(cos(theta), sin(theta), 0.0);
    vec2 dists = raySphere(pos, V);
    vec4 o = intensity(delta, pos + V * dists.x, pos + V * dists.y);

    imageStore(lut, ivec3(gl_GlobalInvocationID.xyz), vec4(dists.y * 100.0, 0.0, 1.0)); 
}
