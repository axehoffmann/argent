#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Resource.h"

#include "lib/string.h"
#include "lib/vector.h"
#include "resources/v2/packer/mesh/mesh.h"

#include <vector>

namespace ag
{
    struct Mesh : public Resource
    { 
        Mesh(const string& path) : Resource(path) {}
        ~Mesh() { Unload(); }


        mesh_ir mesh;

        bool IsReady() const override;
        void Load() override;
        void Unload() override;
    };
}