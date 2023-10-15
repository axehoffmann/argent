#pragma once

#include "Resource.h"
#include "lib/vector.h"

namespace ag
{
    struct AudioClip : public Resource
    {
    public:
        AudioClip(std::string path) : Resource(path) {}
        ~AudioClip() { Unload(); }

        vector<uint16_t> data;

        bool IsReady() const override;
        void Load() override;
        void Unload() override;
    };
}
