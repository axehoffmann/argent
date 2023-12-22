#include "Texture.h"

#include <stb_image.h>

bool ag::Texture::IsReady() const
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
