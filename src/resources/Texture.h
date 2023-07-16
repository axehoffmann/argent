#pragma once

#include "Resource.h"

typedef unsigned char u_char;

namespace ag
{
    struct Texture : public Resource
    {
    public:
        Texture(std::string path) : Resource(path) 
        {
            data = nullptr;
            width = height = channels = 0;
        }
        ~Texture() { Unload(); }

        u_char* data;
        int width;
        int height;
        int channels;

        bool IsReady() const override;
        void Load() override;
        void Unload() override;


    };
}