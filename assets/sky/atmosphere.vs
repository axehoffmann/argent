#version 460 core

uniform mat4 viewInv;
uniform mat4 projInv;

out vec3 viewRay;

// Fullscreen triangle render from 'derhass' on SO
void main()
{
    vec2 vertices[6] = vec2[6](
        vec2(-1, -1), vec2(1, -1), vec2(-1, 1),
        vec2(-1, 1), vec2(1, -1), vec2(1, 1)
    );

    gl_Position = vec4(vertices[gl_VertexID], 0, 1);
    viewRay = (viewInv * vec4((projInv * vec4(vertices[gl_VertexID], 1.0, 0.0)).xyz, 0.0)).xyz;
}
