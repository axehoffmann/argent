#pragma once

#include "../../resources/Texture.h"
#include "GL.h"

#define GLEW_STATIC
#include <GL/glew.h>

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

		GLTexture(ag::Texture* tex, ag::TextureType type, ag::TextureFormat format, bool mipmap = false);
		~GLTexture();

		GLHandle handle;
	};
}