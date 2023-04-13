#include "Texture.h"

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)


bool ag::Texture::IsReady()
{
    return data != nullptr;
}

void ag::Texture::Load()
{
    data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (data == nullptr)
    {
		throw std::runtime_error("Failed loading Texture resource from path " + filePath);
        return;
    }
}

void ag::Texture::Unload()
{
    if (data == nullptr)
        return;

    stbi_image_free(data);
    data = nullptr;
}
