#pragma once

#include "Resource.h"
#include <nlohmann/json.hpp>
#include <vector>

namespace ag
{
    struct Material : public Resource
    {
    public:
        std::vector<uint32_t> textures;
        std::vector<float> parameters;

        bool IsReady();
        void Load();
        void Unload();
    };
}