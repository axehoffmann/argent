#version 460 core

uniform mat4 viewInv;
uniform mat4 projInv;

out vec3 viewRay;

void main()
{
    vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));

    gl_Position = vec4(vertices[gl_VertexID], 0, 1);
    viewRay = (viewInv * vec4((projInv * vec4(vertices[gl_VertexID], 0.0, 0.0)).xyz, 0.0)).xyz;
}
