#pragma once

#include "rendering/ogl/opengl.h"

enum class tex_type : GLenum
{
	Tex2D = GL_TEXTURE_2D,
	Tex2DMS = GL_TEXTURE_2D_MULTISAMPLE,

	Tex3D = GL_TEXTURE_3D,
};

enum class tex_format : GLenum
{
	Depth = GL_DEPTH24_STENCIL8,
	Depth24 = GL_DEPTH_COMPONENT24,
	
	
	R32 = GL_R32F,

	RGBA32 = GL_RGBA32F,
	RGBA16 = GL_RGBA16,
	RGBA8 = GL_RGBA8,
	RGB8 = GL_RGB8,

	RGBA = GL_RGBA
};

enum class tex_filter : GLenum
{
	Linear = GL_LINEAR,
	NearestMip = GL_NEAREST_MIPMAP_NEAREST
};

enum class tex_wrap : GLenum
{
	Clamp = GL_CLAMP,
	Repeat = GL_REPEAT,
};

class texture
{
public:
	texture(tex_filter minFilter = tex_filter::Linear, tex_type texType = tex_type::Tex2D, tex_wrap wrap = tex_wrap::Repeat);
	texture(glhandle h);

	void allocate(u32 w, u32 h, u32 mips, tex_format fmt);
	void allocate(u32 w, u32 h, tex_format fmt, u32 samples);
	void allocate3D(u32 w, u32 h, u32 d, u32 mips, tex_format fmt);

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