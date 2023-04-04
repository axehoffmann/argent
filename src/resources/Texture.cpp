#include "Texture.h"

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
    stbi_image_free(data);
}
