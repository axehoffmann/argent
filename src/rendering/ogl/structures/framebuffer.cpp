#include "framebuffer.h"

framebuffer::framebuffer()
{
	glCreateFramebuffers(1, &id);
}

framebuffer::~framebuffer()
{
	glDeleteFramebuffers(1, &id);
}

glhandle framebuffer::getID() const
{
	return id;
}

void framebuffer::attach(texture& tex, attachment type, u32 offset)
{
	glNamedFramebufferTexture(id, static_cast<GLenum>(type) + offset, tex.getID(), 0);
}

