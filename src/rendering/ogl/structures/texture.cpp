#include "texture.h"
#include "texture.h"
#include "texture.h"
#include "texture.h"

texture::texture()
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

texture::texture(glhandle glID)
	: id(glID)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateTextureMipmap(id);
}

void texture::allocate(u32 w, u32 h, u32 mips, tex_format fmt)
{
	checkError();
	glTextureStorage2D(id, mips, static_cast<GLenum>(fmt), w, h);
	checkError();
}

void texture::setData(u32 w, u32 h, void* data, tex_format fmt)
{
	glTextureStorage2D(id, 1, GL_RGBA8, w, h);
	glTextureSubImage2D(id, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(id);
}

void texture::bind(u32 slot)
{
	glBindTextureUnit(slot, id);
}

u64 texture::makeBindless()
{
	u64 blh = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(blh);
	return blh;
}
