#version 460 core

in vec2 v_UV;
out vec4 fragColour;

layout (binding = 0) uniform sampler2D screenTex;
uniform float clock;

void main()
{
    fragColour = vec4(texture(screenTex, v_UV).xyz, 1.0);
    // fragColour = vec4(texture(screenTex, v_UV).xyz, 1.0);
}
