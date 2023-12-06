#pragma once

#include "vao.h"

vao::vao() :
	handle(-1)
{
	glGenVertexArrays(1, &handle);
}

vao::~vao()
{
	if (handle == -1)
		return;

	glDeleteVertexArrays(1, &handle);
}

vao::vao(vao&& other) :
	handle(other.handle)
{
	other.handle = -1;
}

vao& vao::operator=(vao&& other) noexcept
{
	handle = other.handle;
	other.handle = -1;
	return *this;
}

void vao::bind()
{
	bound = handle;
	glBindVertexArray(handle);
}

void vao::assignAttribute(u32 loc, u64 offset, i32 size, gltype type, i32 stride, bool normalised)
{
	glVertexAttribPointer(loc, size, static_cast<GLenum>(type), normalised, stride, (void*)offset);
	glEnableVertexAttribArray(loc);
}