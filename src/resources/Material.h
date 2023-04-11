#pragma once

#include "Resource.h"
#include "AssetManager.h"
#include "Texture.h"
#include <nlohmann/json.hpp>
#include <vector>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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