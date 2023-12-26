#version 460 core

// Performs culling on input instances, generating a list of all remaining instance IDs, and output draw commands

layout(local_size_x = 256) in;

struct SceneInfo
{
    uint totalObjects;
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
	int mat;
    // AABB?
};
layout (std430, binding = 1) readonly buffer instanceData 
{
	idata instance[]; 
};

struct RenderInstance
{
    uint residentMeshID;
};
layout (std430, binding = 11) readonly buffer allInstances
{
    RenderInstance inputInstances[];
};

////////////
// OUTPUT //
////////////
layout (std430, binding = 12) writeonly buffer outInstances
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
layout (std430, binding = 13) buffer drawCmds
{
    // Generates 1 draw command for each mesh. 
    // Currently we don't bother cutting commands with 0 instances,
    // and hope the overhead is minimal
    IndirectDrawCommand drawCommands[];
};

void main()
{
    // The ID of the object out of all existing renderables
    uint objectID = gl_GlobalInvocationID.x;

    if (instanceID >= sceneInfo.totalObjects)
    {
        discard;
    }

    uint residentMeshID = inputInstances[instanceID].residentMeshID;
    // The index of the instance within its own mesh batch
    uint drawID = atomicAdd(drawCommands[residentMeshID].instanceCount, 1);
    // The index of the instance within all visible objects
    uint outputIndex = drawCommands[residentMeshID].baseInstance + drawID;

    // Create the indirection from the draw instance ID to the actual object's daa
    visibleInstances[outputIndex] = objectID;
}