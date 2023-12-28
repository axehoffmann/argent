#version 460 core

#extension GL_ARB_bindless_texture : require

in vec2 a_UV;
in vec3 a_Norm;
in vec3 fragPos;


const float PI = 3.14159265359;


struct PointLight
{
	vec4 pos;
	vec4 colour;
};

struct Material
{
	uvec2 colour;
	uvec2 norm;
	uvec2 detail;
	uvec2 detail2;
};

// Input buffers
layout(std430, binding = 2) buffer mats
{
	Material materials[];
};

layout(std430, binding = 3) buffer matTable
{
	uint materialTable[];
};

layout(std430, binding = 4) buffer pLights
{
	uint lightCount;
	PointLight pointLights[];
};

flat in int materialID;

uniform vec3 viewPos;
uniform vec3 lightPos;

out vec4 FragColor;

vec3 Fresnel(vec3 V, vec3 H, vec3 F0);
float NDF(float a, vec3 N, vec3 H);
float Geometry(float a, float NdV, float NdL);
float Geometry1(float k, float NdX);
vec3 BRDF(PointLight light, vec3 colour, float r, float m, float NdL, vec3 N, vec3 L, vec3 V, vec3 F0);

vec3 pointLightContribution(vec3 colour, vec3 F0, float r, float m, vec3 N, vec3 V, PointLight light);

void main()
{
	Material mat = materials[materialTable[materialID] - 1];
	vec3 colour = texture(sampler2D(mat.colour), a_UV).rgb;

	vec4 detail = texture(sampler2D(mat.detail), a_UV);
	float r = detail.r;
	float m = detail.g;

	// Dielectrics have F0=0.04, metallics have F0 represented by colour map
	vec3 F0 = mix(vec3(0.04), colour, m);

	vec3 V = viewPos - fragPos;
	vec3 N = normalize(a_Norm);

	vec3 Lo = vec3(0.0);

	for (int i = 0; i < lightCount; i++) {

		Lo += pointLightContribution(colour, F0, r, m, N, V, pointLights[i]);
	}
	
	FragColor = vec4(Lo, 1.0);
}

vec3 pointLightContribution(vec3 colour, vec3 F0, float r, float m, vec3 N, vec3 V, PointLight light)
{
	vec3 L = normalize(light.pos.xyz - fragPos);
	float NdL = max(dot(N, L), 0.0);

	// Incoming light
	float dist = length(light.pos.xyz - fragPos);
	float attenuation = light.colour.a / (dist * dist);
	vec3 irradiance = light.colour.rgb * attenuation * NdL;

	// Outgoing light
	return BRDF(light, colour, r, m, NdL, N, L, V, F0) * irradiance;
}

vec3 BRDF(PointLight light, vec3 colour, float r, float m, float NdL, vec3 N, vec3 L, vec3 V, vec3 F0)
{
	// Cook-Torrance BRDF impl
	float NdV = max(dot(N, V), 0.0);
	vec3 H = normalize(L + V);
	vec3 F = Fresnel(V, H, F0);
	float a = r * r;

	// Diffuse contribution (only for dielectrics)
	vec3 fd = (vec3(1.0) - F) * (1.0 - m) * colour / PI;
	// Specular contribution
	vec3 fs = F * NDF(a , N, H) * Geometry(r, NdV, NdL) / (4.0 * NdL * NdV + 0.00000001);
	
	return fd + fs;
}

vec3 Fresnel(vec3 V, vec3 H, vec3 F0)
{
	// Fresnel-Schlick impl
	return F0 + (1.0 - F0) * pow((clamp(1.0 - dot(V, H), 0.0, 1.0)), 5.0);
}

float NDF(float a, vec3 N, vec3 H)
{
	// Distribution GGX impl
	float a2 = a * a;
	float NdH = max(dot(N, H), 0.0);

	float term = NdH * NdH * (a2 - 1.0) + 1.0;

	return a2 / (PI * term * term);
}

float Geometry1(float k, float NdX)
{
	// Geometry-Schlick-GGX impl
	return NdX / (NdX * (1.0 - k) + k);
}

float Geometry(float a, float NdV, float NdL)
{
	// Geometry-Smith impl
	float k = (a + 1) * (a + 1) / 8.0;
	return Geometry1(k, NdV) * Geometry1(k, NdL);
}