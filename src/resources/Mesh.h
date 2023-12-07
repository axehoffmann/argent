#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Resource.h"

#include "lib/string.h"
#include "lib/vector.h"
#include "rendering/vertex.h"

#include <vector>

namespace ag
{
    struct Mesh : public Resource
    { 
        Mesh(const string& path) : Resource(path) {}
        ~Mesh() { Unload(); }


        vector<vertex> vertices;
        vector<uint32_t> indices;

        bool IsReady() const override;
        void Load() override;
        void Unload() override;
    };
}