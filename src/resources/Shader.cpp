#include "Shader.h"

bool ag::Shader::IsReady() const
{
    return true;
}

void ag::Shader::Load()
{
    json ob = AssetManager::ReadJson(filePath);

    vertexPath = ob["vertex"].get<std::string>();
    fragmentPath = ob["fragment"].get<std::string>();
}

/// TODO: Better scheme for loading and dumping shader data
void ag::Shader::Unload()
{
}
