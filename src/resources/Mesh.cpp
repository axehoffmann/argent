#include "Mesh.h"

uint32_t ag::Mesh::nextID(0);

ag::Mesh::Mesh(std::string filePath)
{
    path = filePath;
    ID = ++nextID;
}

void ag::Mesh::Load()
{
    // Based on vulkan-tutorial.com model loader
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> primitives;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    bool success = tinyobj::LoadObj(&attributes, &primitives, &materials, &warning, &error, path.c_str());

    if (!success)
        throw std::runtime_error(warning + error);

    for (const tinyobj::shape_t& triangle : primitives)
    {
        for (const tinyobj::index_t& index : triangle.mesh.indices)
        {
            Vertex vertex{};

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

#if AG_INDEXED_LOAD
            if (uniqueVertices.count(vertex) == 0) 
#endif
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }


            indices.push_back(uniqueVertices[vertex]);
        }

        for (size_t i = 2; i < indices.size(); i += 3)
        {
            Vertex* vertex1 = &(vertices.at(indices.at(i - 2)));
            Vertex* vertex2 = &(vertices.at(indices.at(i - 1)));
            Vertex* vertex3 = &(vertices.at(indices.at(i - 0)));

            // Calculate tangents and bitangents
            glm::vec3 edge1 = vertex2->position - vertex1->position;
            glm::vec3 edge2 = vertex3->position - vertex1->position;
            glm::vec2 deltaUV1 = vertex2->uv - vertex1->uv;
            glm::vec2 deltaUV2 = vertex3->uv - vertex1->uv;

            glm::vec3 tangent, bitangent;
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            vertex1->tangent = vertex2->tangent = vertex3->tangent = tangent;
            vertex1->bitangent = vertex2->bitangent = vertex3->bitangent = bitangent;
        }
    }
}

void ag::Mesh::Unload()
{
    // Free mesh memory
    std::vector<Vertex>().swap(vertices);
    std::vector<uint32_t>().swap(indices);
}
