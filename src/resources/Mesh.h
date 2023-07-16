#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Resource.h"

#include <unordered_map>
#include <vector>

#define AG_INDEXED_LOAD 0

namespace ag
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        bool operator==(const Vertex& other) const
        {
            return position == other.position && uv == other.uv;
        }
    };

    struct Mesh : public Resource
    { 
        Mesh(std::string path) : Resource(path) {}
        ~Mesh() { Unload(); }


        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        bool IsReady() const override;
        void Load() override;
        void Unload() override;
    };
}


namespace std
{
    /**
     * Vertex hash implementation for use in hash sets
    */
    template<> struct hash<ag::Vertex>
    {
        size_t operator()(ag::Vertex const& vertex) const
        {
            return (hash<glm::vec3>()(vertex.position)) ^
                (hash<glm::vec2>()(vertex.uv) << 1) ^
                (hash<glm::vec3>()(vertex.normal) << 2);
        }
    };
}