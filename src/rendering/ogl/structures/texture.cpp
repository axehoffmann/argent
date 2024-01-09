#include "texture.h"
#include "texture.h"
#include "texture.h"
#include "texture.h"

texture::texture(tex_filter minFilter)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minFilter));
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

texture::texture(glhandle glID)
	: id(glID)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	f32 aniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTextureParameterf(id, GL_TEXTURE_MAX_ANISOTROPY, aniso);
}

void texture::allocate(u32 w, u32 h, u32 mips, tex_format fmt)
{
	glTextureStorage2D(id, mips, static_cast<GLenum>(fmt), w, h);
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
