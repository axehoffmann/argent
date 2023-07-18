#pragma once

#include "../../resources/Texture.h"
#include "GL.h"

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

		GLTexture(std::shared_ptr<ag::Texture> tex, ag::TextureType type, ag::TextureFormat format, bool mipmap = false);
		~GLTexture();

		void Bind(int slot);

		GLHandle handle;
		ag::TextureType texType;
	};
}