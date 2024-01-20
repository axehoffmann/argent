#version 460 core

in vec2 v_UV;
out vec4 fragColour;

layout (binding = 0) uniform sampler3D scatterLUT;

uniform vec3 camPos;

in vec3 viewRay;

const float PI = 3.14159265359;

const vec3 lutSize = vec3(128, 512, 512);

const vec3 rayleighScattering = vec3(0.005802, 0.013558, 0.033100);
const vec3 mieScattering = vec3(0.003996, 0.003996, 0.003996);

const float Rp = 6360.000000;
const float Ra = 6420.000000;

const float g = 0.8;

const float exposure = 0.5;
const vec3 whitePoint = vec3(1.0);
uniform vec3 sunDirection;

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
    return (3.0 / (16.0 * PI)) * (1 + cosTheta * cosTheta);
}

// Adjusted F_R(theta) from Elek (2009)
float phaseRayleigh2(float cosTheta)
{
    return 0.8 * (1.4 + 0.5 * cosTheta);
}

vec3 extrapolateMieScatter(vec4 scattering)
{
    if (scattering.r <= 0.0)
    {
        return vec3(0.0);
    }
    return scattering.rgb * scattering.a / scattering.r *
        (rayleighScattering.r / mieScattering.r) *
        (mieScattering / rayleighScattering);
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
vec3 RMuMusToUVW(float r, float mu, float muS)
{
    float u = clamp(r, 0.0, Ra - Rp);
    u = pow(clamp(u / (Ra - Rp), 0.0, 1.0), 0.5);

    float v;
    r = max(r, 0.0);
    float cosH = -sqrt(r * (2.0 * Rp + r)) / (Rp + r);
    if (mu > cosH)
    {
        mu = max(mu, cosH + 1e-4);
        v = clamp((mu - cosH) / (1.0 - cosH), 0.0, 1.0);
        v = pow(v, 0.2);
        v = 0.5 + 0.5 / lutSize.y + v * (lutSize.y / 2.0 - 1.0) / lutSize.y;
    }
    else
    {
        mu = min(mu, cosH - 1e-4);
        v = clamp((cosH - mu) / (cosH + 1.0), 0.0, 1.0);
        v = pow(v, 0.2);
        v = 0.5 / lutSize.y + v * (lutSize.y / 2.0 - 1.0) / lutSize.y;
    }

    float w = 0.5 * (atan(max(muS, -0.45f) * tan(1.26 * 0.75)) / 0.75 + (1.0 - 0.26));

    vec3 uvw = vec3(u, v, w);
    uvw.xz = ((uvw * (lutSize - 1.0) + 0.5) / lutSize).xz;
    return uvw;
}

vec3 getScattering(float r, float mu, float muS, float nu, bool rayHitsGround, out vec3 mieScatter)
{
    vec3 uvw = RMuMusToUVW(r, mu, muS);

    vec4 sc = texture(scatterLUT, uvw);
    mieScatter = extrapolateMieScatter(sc);
    return vec3(sc);
}

bool rayRMuHitsGround(float r, float mu)
{
    return mu < 0.0 && r*r * (mu*mu - 1.0) + Rp*Rp >= 0.0;
}

vec3 getSkyRadiance(vec3 position, vec3 ray, float shadowLen, vec3 sunDir, out vec3 transmittance)
{
    float r = length(position);
    float rMu = dot(position, ray);
    float distToTop = -rMu - sqrt(rMu*rMu - r*r + Ra*Ra);

    float mu = rMu / r;
    float muS = dot(position, sunDir) / r;
    float nu = dot(ray, sunDir);
    bool rayHitsGround = rayRMuHitsGround(r, mu);

    vec3 scattering, mieScatter;
    
    // Only handle unshadowed case for now
    scattering = getScattering(r, mu, muS, nu, rayHitsGround, mieScatter);

    return scattering * phaseRayleigh(nu) + mieScatter * phaseMie(nu);
}


void main()
{
    vec3 viewDir = normalize(viewRay);
    float fragmentRadius = length(dFdx(viewRay) + dFdy(viewRay)) / length(viewRay);
    
    vec3 transmittance;
    vec3 radiance = getSkyRadiance(camPos, viewDir, 0.0, sunDirection, transmittance);

    vec3 col = pow(vec3(1.0) - exp(-radiance / whitePoint * exposure), vec3(1.0 / 2.2));
    fragColour = vec4(col, 1.0);
}
