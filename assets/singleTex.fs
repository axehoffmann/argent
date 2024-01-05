#version 460 core

in vec2 v_UV;
out vec4 fragColour;

layout (binding = 0) uniform sampler2D screenTex;

void main()
{
    fragColour = texture(screenTex, v_UV);
    // fragColour = vec4(1.0);
}