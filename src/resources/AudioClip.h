#pragma once

#include "Resource.h"
#include <vector>

namespace ag
{
    struct AudioClip : public Resource
    {
    public:
        AudioClip(std::string path) : Resource(path) {}
        ~AudioClip() { Unload(); }

        std::vector<uint16_t> data;

        bool IsReady() override;
        void Load() override;
        void Unload() override;
    };
}
