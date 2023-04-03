#include "Texture.h"

std::atomic<uint32_t> ag::Texture::nextID(0);


ag::Texture::Texture(std::string filePath)
{
    path = filePath;
    ID = ++nextID;
}

bool ag::Texture::IsReady()
{
    return data != nullptr;
}

void ag::Texture::Load()
{
    data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (data == nullptr)
    {
		throw std::runtime_error("Failed loading Texture resource from path " + path);
        return;
    }
}

void ag::Texture::Unload()
{
    stbi_image_free(data);
}
