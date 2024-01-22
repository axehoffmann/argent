#version 460 core

uniform mat4 viewInv;
uniform mat4 projInv;

uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;

out vec3 viewRay;

void main()
{
    vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));

    gl_Position = vec4(vertices[gl_VertexID], 0, 1);


    const float w = 1280;
    const float h = 720;
    vec2 uv = vertices[gl_VertexID] * 0.5 + 0.5;

    vec3 camRight = camUp * camDir;

    viewRay = uv.x * camRight + uv.y * camUp + camDir;
    
    // viewRay = (viewInv * vec4((projInv * vec4(vertices[gl_VertexID], 1.0, 0.0)).xyz, 0.0)).xyz;
}
