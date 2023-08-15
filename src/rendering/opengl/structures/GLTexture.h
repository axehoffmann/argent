#pragma once

#include "resources/Texture.h"
#include "resources/AssetManager.h"
#include "rendering/opengl/GL.h"

#include <memory>
#include <string>

namespace ag
{
	enum class TextureType
	{
		Tex2D = GL_TEXTURE_2D
	};
	enum class TextureFormat
	{
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};
	class GLTexture
	{
	public:

		GLTexture(uint32_t resourceID, ag::TextureType type, ag::TextureFormat format, bool mipmap = false);
		~GLTexture();

		void Bind(int slot);

		GLHandle handle;
		ag::TextureType texType;
	};
}