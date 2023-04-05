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
        u_char* data;
        int width;
        int height;
        int channels;

        bool IsReady();
        void Load();
        void Unload();


    };
}