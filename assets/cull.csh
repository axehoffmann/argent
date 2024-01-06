#version 460 core

// Performs culling on input instances, generating a list of all remaining instance IDs, and output draw commands

layout(local_size_x = 256) in;

layout(binding = 0) uniform sampler2D hzb;

struct SceneInfo
{
    mat4 view;
    uint totalObjects;

    // Frustum culling data
    float frustum[4];

    // Occlusion culling data
    float proj_00, proj_11;
    float zNear, zFar;
    float hzbWidth, hzbHeight;
};

layout (std430, binding = 10) readonly buffer sceneCullingData 
{
    SceneInfo sceneInfo;
};

///////////
// INPUT //
///////////
struct idata 
{
	mat4 model;
    vec4 boundingSphere;
	uint mat;
    uint residentMeshID;
};
layout (std430, binding = 1) readonly buffer instanceData 
{
	idata instances[]; 
};


////////////
// OUTPUT //
////////////
layout (std430, binding = 11) writeonly buffer outInstances
{
    // An indireciton from draw InstanceID to engine ObjectID (index into instance data)
    uint visibleInstances[];
};
// Draw command output
struct IndirectDrawCommand
{
    uint indexCount;
    uint instanceCount;
    uint firstIndex;
    uint baseVertex;
    uint baseInstance;
};
layout (std430, binding = 12) buffer drawCmds
{
    // Generates 1 draw command for each mesh. 
    // Currently we don't bother cutting commands with 0 instances,
    // and hope the overhead is minimal
    IndirectDrawCommand drawCommands[];
};

// 2D Polyhedral Bounds of a Clipped, Perspective-Projected 3D Sphere. Michael Mara, Morgan McGuire. 2013
bool projectSphereToScreenSpace(vec3 C, float r, out vec4 aabb)
{
    if (C.z < r + sceneInfo.zNear)
        return false;

    vec2 cx = -C.xz;
    vec2 vx = vec2(sqrt(dot(cx, cx) - r * r), r);
    vec2 minX = mat2(vx.x, -vx.y, vx.y, vx.x) * cx;
    vec2 maxX = mat2(vx.x, vx.y, -vx.y, vx.x) * cx;

    vec2 cy = -C.yz;
    vec2 vy = vec2(sqrt(dot(cy, cy) - r * r), r);
    vec2 minY = mat2(vy.x, vy.y, -vy.y, vy.x) * cy;
    vec2 maxY = mat2(vy.x, -vy.y, vy.y, vy.x) * cy;

    aabb = vec4(minX.x / minX.y * sceneInfo.proj_00, minY.x / minY.y * sceneInfo.proj_11, maxX.x / maxX.y * sceneInfo.proj_00, maxY.x / maxY.y * sceneInfo.proj_11);
    aabb = aabb.xwzy * vec4(0.5, -0.5, 0.5, -0.5) + vec4(0.5);

    return true;
}

bool isVisible(uint objectID)
{
    // Cull against camera frustum
    vec4 boundingSphere = instances[objectID].boundingSphere;

    vec3 center = boundingSphere.xyz;
    // Translate sphere to view space
    center = (sceneInfo.view * vec4(center, 1.0)).xyz;

    float radius = boundingSphere.w;

    bool visible = true;
    visible = visible && center.z * sceneInfo.frustum[1] - abs(center.x) * sceneInfo.frustum[0] > -radius;
    visible = visible && center.z * sceneInfo.frustum[3] - abs(center.y) * sceneInfo.frustum[2] > -radius;

    /// TODO: missing forward/back culling?

    // Cull against last frame's depth buffer
    center.y *= -1;
    if (visible)
    {
        vec4 aabb;
        if (projectSphereToScreenSpace(center, radius, aabb))
        {
            float w = (aabb.z - aabb.x) * sceneInfo.hzbWidth;
            float h = (aabb.w - aabb.y) * sceneInfo.hzbHeight;

            float hzbLod = floor(log2(max(w, h)));

            float depth = textureLod(hzb, (aabb.xy + aabb.zw) * 0.5, hzbLod).x;
            float depthSphere = sceneInfo.zNear / (center.z - radius);

            visible = visible && depthSphere >= depth;
        }
    }

    return visible;
}

void main()
{
    // The ID of the object out of all existing renderables
    uint objectID = gl_GlobalInvocationID.x;

    if (objectID >= sceneInfo.totalObjects)
    {
        return;
    }

    if (!isVisible(objectID))
    {
        return;
    }

    uint residentMeshID = instances[objectID].residentMeshID;
    // The index of the instance within its own mesh batch
    uint drawID = atomicAdd(drawCommands[residentMeshID].instanceCount, 1);
    // The index of the instance within all visible objects
    uint outputIndex = drawCommands[residentMeshID].baseInstance + drawID;

    // Create the indirection from the draw instance ID to the actual object's daa
    visibleInstances[outputIndex] = objectID;
}