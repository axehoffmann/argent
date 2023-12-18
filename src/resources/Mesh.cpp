#include "Mesh.h"

#include <iostream>


bool ag::Mesh::IsReady() const
{
    return mesh.vertices.size() > 0;
}

void ag::Mesh::Load()
{
    mesh = load_mesh(filePath);
}

void ag::Mesh::Unload()
{
    if (mesh.vertices.size() == 0 && mesh.indices.size() == 0)
        return;
}