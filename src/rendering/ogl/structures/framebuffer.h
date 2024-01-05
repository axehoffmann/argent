#pragma once

#include "rendering/ogl/opengl.h"
#include "texture.h"

enum class attachment : GLenum
{
	Colour = GL_COLOR_ATTACHMENT0,
	Depth = GL_DEPTH_ATTACHMENT
};

class framebuffer
{
public:
	framebuffer();
	~framebuffer();

	glhandle getID() const;
	void attach(texture& tex, attachment type, u32 offset = 0);
private:
	glhandle id;
};