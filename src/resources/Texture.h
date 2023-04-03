#pragma once

#include <string>
#include <atomic>
#include <stdexcept>

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

namespace ag
{
    struct Texture
    {
    public:
        u_char* data;
        int width;
        int height;
        int channels;

        uint32_t ID;
        std::string path;

        Texture(std::string filePath);

        bool IsReady();
        void Load();
        void Unload();

    private:
        static std::atomic<uint32_t> nextID;
    };
}