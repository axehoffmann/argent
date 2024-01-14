#pragma once

#include "rendering/ogl/opengl.h"

enum class tex_type : GLenum
{
	Tex2D = GL_TEXTURE_2D,
	Tex2DMS = GL_TEXTURE_2D_MULTISAMPLE,
};

enum class tex_format : GLenum
{
	Depth = GL_DEPTH24_STENCIL8,
	Depth24 = GL_DEPTH_COMPONENT24,
	
	
	R32 = GL_R32F,

	RGBA8 = GL_RGBA8,
	RGB8 = GL_RGB8,

	RGBA = GL_RGBA
};

enum class tex_filter : GLenum
{
	Linear = GL_LINEAR,
	NearestMip = GL_NEAREST_MIPMAP_NEAREST
};

class texture
{
public:
	texture(tex_filter minFilter = tex_filter::Linear, tex_type texType = tex_type::Tex2D);
	texture(glhandle h);

	void allocate(u32 w, u32 h, u32 mips, tex_format fmt);
	void allocate(u32 w, u32 h, tex_format fmt, u32 samples);

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