#version 460 core

#extension GL_ARB_bindless_texture : require

in vec2 a_UV;
in vec3 a_Norm;
in vec3 fragPos;

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

flat in int materialID;

uniform vec3 viewPos;
uniform vec3 lightPos;

out vec4 FragColor;

void main()
{
	Material mat = materials[materialTable[materialID]];

	vec3 V = viewPos - fragPos;
	vec3 L = lightPos - fragPos;
	vec3 H = normalize(L + V);

	float dist = length(L);
	L = normalize(L);

	dist = dist * dist;

	float NdL = dot(a_Norm, L);
	float intensity = max(NdL, 0.0);

	vec3 diff = intensity * vec3(texture(sampler2D(materials[materialID].alb), a_UV)) * 10.0 / dist;

	float NdH = dot(a_Norm, H);
	intensity = pow(max(NdH, 0.0),  4.0);

	vec3 spec = intensity * vec3(1.0, 0.7, 0.7) * 1.0 / dist;
	
	FragColor = vec4(diff + spec, 1.0);
	// FragColor = vec4(1.0);
}