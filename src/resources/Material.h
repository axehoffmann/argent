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
        Material(std::string path) : Resource(path) {}
        ~Material() { Unload(); }

        /**
        * The IDs of the textures that comprise the material
        */
        std::vector<uint32_t> textures;
        /// TODO: change parameters to accept different types
        std::vector<float> parameters;

        bool IsReady() const override;
        void Load() override;
        void Unload() override;
    };
}