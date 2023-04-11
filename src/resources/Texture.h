#pragma once

#include "Resource.h"


#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

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

        bool IsReady() override;
        void Load() override;
        void Unload() override;


    };
}