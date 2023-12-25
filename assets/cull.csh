#version 460 core

layout(local_size_x = 256) in;

struct SceneInfo
{
    uint totalObjects;
};

layout (std430, binding = 10) readonly buffer sceneCullingData 
{
    SceneInfo sceneInfo;
};

struct RenderInstance
{
    uint meshID;
};

layout (std430, binding = 11) readonly buffer allInstances
{
    RenderInstance inputInstances[];
};


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
    IndirectDrawCommand drawCommands[];
};

void main()
{
    uint instanceID = gl_GlobalInvocationID.x;

    if (instanceID >= sceneInfo.totalObjects)
    {
        discard;
    }


}