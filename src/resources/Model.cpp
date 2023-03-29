#include "Model.h"

uint32_t ag::Model::nextID(0);

ag::Model::Model(std::string filePath)
{
    path = filePath;
    ID = ++nextID;
}

void ag::Model::Load()
{
    /// TODO: Implement
}

void ag::Model::Unload()
{
    // Free model memory
    std::vector<Vertex>().swap(vertices);
    std::vector<uint32_t>().swap(indices);
}
