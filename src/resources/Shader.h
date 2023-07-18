#pragma once

#include "Resource.h"
#include "AssetManager.h"
#include <nlohmann/json.hpp>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace ag
{
    struct Shader : public Resource
    {
    public:
        Shader(std::string path) : Resource(path) {}
        ~Shader() { Unload(); }

        bool IsReady() const override;
        void Load() override;
        void Unload() override;

        std::string vertexPath;
        std::string fragmentPath;
    };
}