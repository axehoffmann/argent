#include "mesh.h"

#include "debug/log/Log.h"

#include <fstream>
#include <iostream>

#include <bitsery/adapter/stream.h>

#pragma warning(push, 0)
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#pragma warning(pop)

void compile_mesh(const string& path)
{
    string outPath = path.substr(0, path.find('.')) + ".agmesh";
    mesh_ir opt = optimize_mesh(load_obj(path));
    save_mesh(opt, outPath);
}

mesh_ir load_obj(const string& path)
{
    tinyobj::attrib_t attributes;
    vector<tinyobj::shape_t> primitives;
    vector<tinyobj::material_t> materials;
    string warning, error;

    mesh_ir ir{};
    
    bool success = tinyobj::LoadObj(&attributes, &primitives, &materials, &warning, &error, path.c_str());
    
    if (!success)
    {
        std::cout << "Failed to load mesh " << path << "\n";
        return ir;
    }

    u64 tris = primitives.size();

    for (const tinyobj::shape_t& triangle : primitives)
    {
        for (const tinyobj::index_t& index : triangle.mesh.indices)
        {
            basic_vertex vertex{};

            u64 positionIndex   = 3 * u64(index.vertex_index);
            u64 uvIndex         = 2 * u64(index.texcoord_index);
            u64 normalIndex     = 3 * u64(index.normal_index);

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

            ir.vertices.push_back(vertex);
            ir.indices.push_back(ir.indices.size());
        }
    }
	return ir;
}

mesh_ir optimize_mesh(mesh_ir&& m)
{
    // Indexing
    u64 indexCount = m.indices.size();
    vector<u32> remapTable(indexCount);
    u64 vertexCount = meshopt_generateVertexRemap(
        remapTable.data(), 
        m.indices.data(),
        indexCount,
        m.vertices.data(),
        m.vertices.size(),
        sizeof(basic_vertex)
    );

    mesh_ir ir{};
    ir.indices.resize(indexCount);
    ir.vertices.resize(vertexCount);
    meshopt_remapIndexBuffer(ir.indices.data(), nullptr, indexCount, remapTable.data());
    meshopt_remapVertexBuffer(ir.vertices.data(), m.vertices.data(), vertexCount, sizeof(basic_vertex), remapTable.data());

    // Optimization
    meshopt_optimizeVertexCache(ir.indices.data(), ir.indices.data(), indexCount, vertexCount);
    meshopt_optimizeOverdraw(ir.indices.data(), ir.indices.data(), indexCount, &ir.vertices[0].x, vertexCount, sizeof(basic_vertex), 1.05f);
    meshopt_optimizeVertexFetch(ir.vertices.data(), ir.indices.data(), indexCount, ir.vertices.data(), vertexCount, sizeof(basic_vertex));

    return m;
}

void save_mesh(mesh_ir& m, const string& path)
{
    std::fstream s{ path, s.binary | s.trunc | s.out };
    if (!s.is_open())
    {
        ag::Log::Error("Could not write to file " + path);
        return;
    }

    bitsery::Serializer<bitsery::OutputBufferedStreamAdapter> out{ s };
    out.object(m);
    out.adapter().flush();
}

mesh_ir load_mesh(const string& path)
{
    std::fstream s{ path, s.binary | s.in };
    if (!s.is_open())
    {
        ag::Log::Error("Could not open mesh file " + path);
        return mesh_ir();
    }
    
    mesh_ir m;
    auto [e, r] = bitsery::quickDeserialization<bitsery::InputStreamAdapter>(s, m);

    if (e != bitsery::ReaderError::NoError || !r)
    {
        ag::Log::Error("Failed while reading mesh " + path);
        return mesh_ir();
    }

    return m;
}

