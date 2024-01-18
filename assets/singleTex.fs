#version 460 core

in vec2 v_UV;
out vec4 fragColour;

layout (binding = 0) uniform sampler3D screenTex;


void main()
{
    fragColour = vec4(texture(screenTex, vec3(v_UV, 0.2)).xyz, 1.0);
}
