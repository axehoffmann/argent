#include "Mesh.h"

#include <iostream>

#pragma warning(push, 0)
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#pragma warning(pop)


bool ag::Mesh::IsReady() const
{
    return vertices.size() > 0;
}

void ag::Mesh::Load()
{
    // Based on vulkan-tutorial.com model loader
    tinyobj::attrib_t attributes;
    vector<tinyobj::shape_t> primitives;
    vector<tinyobj::material_t> materials;
    string warning, error;

    bool success = tinyobj::LoadObj(&attributes, &primitives, &materials, &warning, &error, filePath.c_str());

    u64 tris = primitives.size();

    if (!success)
    {
        std::cout << "Failed to load mesh " << filePath << "\n";
        return;
    }

    for (const tinyobj::shape_t& triangle : primitives)
    {
        for (const tinyobj::index_t& index : triangle.mesh.indices)
        {
            basic_vertex vertex{};

            u64 positionIndex = 3 * u64(index.vertex_index);
            u64 uvIndex = 2 * u64(index.texcoord_index);
            u64 normalIndex = 3 * u64(index.normal_index);

            vertex = {
                attributes.vertices[positionIndex + 0],
                attributes.vertices[positionIndex + 1],
                attributes.vertices[positionIndex + 2],

                attributes.texcoords[uvIndex + 0],
                1.0f - attributes.texcoords[uvIndex + 1],

                attributes.normals[normalIndex + 0],
                attributes.normals[normalIndex + 1],
                attributes.normals[normalIndex + 2]
            };

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }
}

void ag::Mesh::Unload()
{
    if (vertices.size() == 0 && indices.size() == 0)
        return;
}