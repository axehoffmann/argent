#version 460 core

layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec2 v_UV;
layout (location = 2) in vec3 v_Norm;

layout(std430, binding = 1) buffer data 
{
	mat4 model[];
};

uniform mat4 view;
uniform mat4 proj;

out vec2 a_UV;
out vec3 a_Norm;

out vec3 fragPos;

void main()
{
	mat4 m = model[gl_InstanceID + gl_BaseInstance];

	fragPos = vec3(m * vec4(v_Pos, 1.0));
	gl_Position = proj * view * m * vec4(v_Pos, 1.0);
	a_UV = v_UV;
	a_Norm = mat3(transpose(inverse(m))) * v_Norm;
}