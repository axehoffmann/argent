#pragma once

#include "rendering/ogl/opengl.h"

enum class tex_type : GLenum
{
	Tex2D = GL_TEXTURE_2D
};

class texture
{
public:
	texture();

	void bind(u32 slot);

	void setData(u32 w, u32 h, void* data);

private:
	glhandle handle;
};