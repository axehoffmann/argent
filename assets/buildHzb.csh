#version 460 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

uniform vec2 texSize;

layout (binding = 0) uniform sampler2D inTex;
layout (binding = 1, r32f) uniform writeonly image2D outTex;

void main()
{
    uvec2 pos = gl_GlobalInvocationID.xy;

    float depth = texture(inTex, (vec2(pos) + vec2(0.5)) / texSize).x;

    imageStore(outTex, ivec2(pos), vec4(depth));
}