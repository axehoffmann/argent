#include "meshpk.h"

#pragma warning(push, 0)
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#pragma warning(pop)

mesh_ir load_mesh(const string& path)
{
    tinyobj::attrib_t attributes;
    vector<tinyobj::shape_t> primitives;
    vector<tinyobj::material_t> materials;
    string warning, error;

    mesh_ir ir{};

    bool success = tinyobj::LoadObj(&attributes, &primitives, &materials, &warning, &error, path.c_str());
    
    ir.tris = primitives.size();

    if (!success)
    {
        std::cout << "Failed to load mesh " << path << "\n";
        return;
    }

    for (const tinyobj::shape_t& triangle : primitives)
    {
        for (const tinyobj::index_t& index : triangle.mesh.indices)
        {
            vertex vertex{};

            int positionIndex = 3 * index.vertex_index;
            vertex.position = {
                attributes.vertices[positionIndex + 0],
                attributes.vertices[positionIndex + 1],
                attributes.vertices[positionIndex + 2]
            };

            int uvIndex = 2 * index.texcoord_index;
            vertex.uv = {
                attributes.texcoords[uvIndex + 0],
                1.0f - attributes.texcoords[uvIndex + 1]
            };

            int normalIndex = 3 * index.normal_index;
            vertex.normal = {
                attributes.normals[normalIndex + 0],
                attributes.normals[normalIndex + 1],
                attributes.normals[normalIndex + 2]
            };

            ir.vertices.push_back(vertex);
        }
    }
	return ir;
}

void pack_mesh(const string& path)
{
    mesh_ir mesh = load_mesh(path);

    // Indexing
    u64 indexCount = mesh.tris * 3;
    vector<u32> remapTable(indexCount);
    u64 vertexCount = meshopt_generateVertexRemap(
        remapTable.data(), 
        nullptr, 
        indexCount, 
        mesh.vertices.data(),
        mesh.vertices.size(),
        sizeof(vertex)
    );

    mesh_ir ir{};
    ir.indices.resize(indexCount);
    ir.vertices.resize(vertexCount);
    meshopt_remapIndexBuffer(ir.indices.data(), nullptr, indexCount, remapTable.data());
    meshopt_remapVertexBuffer(ir.vertices.data(), nullptr, vertexCount, sizeof(vertex), remapTable.data());

    // Optimization
    meshopt_optimizeVertexCache(ir.indices.data(), ir.indices.data(), indexCount, vertexCount);
    meshopt_optimizeOverdraw(ir.indices.data(), ir.indices.data(), indexCount, &ir.vertices[0].position.x, vertexCount, sizeof(vertex), 1.05f);
    meshopt_optimizeVertexFetch(ir.vertices.data(), ir.indices.data(), indexCount, ir.vertices.data(), vertexCount, sizeof(vertex));

    

}
