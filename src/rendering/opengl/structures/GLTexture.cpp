#include "GLTexture.h"
#include "GLTexture.h"

ag::GLTexture::GLTexture(uint32_t resourceID, ag::TextureType type, ag::TextureFormat format, bool mipmap)
{
	std::shared_ptr<ag::Texture> tex = ag::AssetManager::Fetch<ag::Texture>(resourceID).lock();
	/// TODO: should get textureformat from source texture resource
	texType = type;

	glGenTextures(1, &handle);
	glBindTexture((GLEnum)type, handle);

	glTexParameteri((GLEnum)type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri((GLEnum)type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri((GLEnum)type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri((GLEnum)type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D((GLEnum)type, 0, (GLEnum)format, tex->width, tex->height, 0, (GLEnum)format, GL_UNSIGNED_BYTE, tex->data);

	if (mipmap)
		glGenerateMipmap((GLEnum)type);
}

ag::GLTexture::GLTexture(GLTexture&& other) : handle(other.handle)
{
	other.handle = GLHandle(-1);
}

void ag::GLTexture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture((GLEnum)texType, handle);
}

ag::GLTexture::~GLTexture()
{
	if (handle != GLHandle(-1))
		glDeleteTextures(1, &handle);
}
