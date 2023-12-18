#version 460 core

#extension GL_ARB_bindless_texture : require

in vec2 a_UV;
in vec3 a_Norm;
in vec3 fragPos;

struct PointLight
{
	vec4 pos;
	vec4 colour;
};

struct Material
{
	uvec2 alb;
	uvec2 norm;
	uvec2 detail;
	uvec2 detail2;
};

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

vec3 pointLightContribution(Material mat, vec3 V, PointLight light)
{
	vec3 L = vec3(light.pos) - fragPos;
	vec3 H = normalize(L + V);

	float dist = length(L);
	L = normalize(L);

	dist = dist * dist;

	float NdL = dot(a_Norm, L);
	float intensity = max(NdL, 0.0);

	vec3 diff = intensity * vec3(texture(sampler2D(mat.alb), a_UV)) * 5.0 / dist;

	float NdH = dot(a_Norm, H);
	intensity = pow(max(NdH, 0.0),  4.0);

	vec3 spec = intensity * vec3(light.colour) * 1.0 / dist;
	return diff + spec;
}

void main()
{
	Material mat = materials[materialTable[materialID] - 1];

	vec3 V = viewPos - fragPos;
	
	vec3 c = vec3(0.0);

	for (int i = 0; i < lightCount; i++)
	{
		c += pointLightContribution(mat, V, pointLights[i]);
	}
	
	FragColor = vec4(c, 1.0);
	// FragColor = vec4(1.0);
}