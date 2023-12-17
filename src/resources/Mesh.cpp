#include "Mesh.h"

#include <iostream>


bool ag::Mesh::IsReady() const
{
    return mesh.vertices.size() > 0;
}

void ag::Mesh::Load()
{
    mesh_ir unop = load_obj(filePath);
    mesh = optimize_mesh(std::move(unop));
}

void ag::Mesh::Unload()
{
    if (mesh.vertices.size() == 0 && mesh.indices.size() == 0)
        return;
}