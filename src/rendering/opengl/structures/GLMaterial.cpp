#include "GLMaterial.h"

#include "resources/Material.h"

ag::GLMaterial::GLMaterial(uint32_t material)
{
	auto mat = AssetManager::Fetch<Material>(material).lock();
	for (u32 tex : mat->textures)
	{
		textures.push_back(GLTexture(tex, TextureType::Tex2D, TextureFormat::RGB, true));
	}
}

void ag::GLMaterial::Bind(const vector<u8>& slots)
{
	for (u8 idx = 0; auto & tex : textures)
	{
		tex.Bind(slots[idx]);
		
		idx++;
	}
}
