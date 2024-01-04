#version 460 core

// Performs culling on input instances, generating a list of all remaining instance IDs, and output draw commands

layout(local_size_x = 256) in;

struct SceneInfo
{
    mat4 view;
    uint totalObjects;

    // Frustum culling data
    float frustum[4];
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

bool isVisibleFrustum(uint objectID)
{
    vec4 boundingSphere = instances[objectID].boundingSphere;

    vec3 center = boundingSphere.xyz;
    // Translate sphere to view space
    center = (sceneInfo.view * vec4(center, 1.0)).xyz;

    float radius = boundingSphere.w;

    bool visible = true;
    visible = visible && center.z * sceneInfo.frustum[1] - abs(center.x) * sceneInfo.frustum[0] > -radius;
    visible = visible && center.z * sceneInfo.frustum[3] - abs(center.y) * sceneInfo.frustum[2] > -radius;

    /// TODO: missing forward/back culling?

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

    if (!isVisibleFrustum(objectID))
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