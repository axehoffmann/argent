#version 460 core

layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec2 v_UV;
layout (location = 2) in vec3 v_Norm;

struct idata 
{
	mat4 model;
	int mat;
};

layout(std430, binding = 1) buffer data 
{
	idata instance[]; 
};

uniform mat4 view;
uniform mat4 proj;

out vec2 a_UV;
out vec3 a_Norm;

flat out int materialID;

out vec3 fragPos;

void main()
{
	idata i = instance[gl_InstanceID + gl_BaseInstance];

	fragPos = vec3(i.model * vec4(v_Pos, 1.0));
	gl_Position = proj * view * i.model * vec4(v_Pos, 1.0);
	a_UV = v_UV;
	a_Norm = mat3(transpose(inverse(i.model))) * v_Norm;

	materialID = i.mat;
}