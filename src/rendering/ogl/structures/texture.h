#pragma once

#include "rendering/ogl/opengl.h"

enum class tex_type : GLenum
{
	Tex2D = GL_TEXTURE_2D
};

enum class tex_format : GLenum
{
	Depth = GL_DEPTH24_STENCIL8,

	RGBA8 = GL_RGBA8,
	RGB8 = GL_RGB8,

	RGBA = GL_RGBA
};

class texture
{
public:
	texture();
	texture(glhandle h);

	void allocate(u32 w, u32 h, u32 mips, tex_format fmt);

	void bind(u32 slot);

	void setData(u32 w, u32 h, void* data, tex_format fmt = tex_format::RGBA8);
	
	glhandle getID() const
	{
		return id;
	}

	u64 makeBindless();

private:
	glhandle id;
};