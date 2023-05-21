#include "GLTexture.h"

ag::GLTexture::GLTexture(ag::Texture* tex, ag::TextureType type, ag::TextureFormat format, bool mipmap = false)
{
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D((GLEnum)type, 0, (GLEnum)format, tex->width, tex->height, 0, (GLEnum)format, GL_UNSIGNED_BYTE, tex->data);

	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
}

ag::GLTexture::~GLTexture()
{
	glDeleteTextures(1, &handle);
}
