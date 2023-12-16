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

vao::vao(vao&& other) noexcept :
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
	glBindVertexArray(handle);
}

void vao::assignAttribute(u32 loc, u64 offset, i32 size, gltype type, i32 stride, bool normalised)
{
	glVertexAttribPointer(loc, size, static_cast<GLenum>(type), normalised, stride, (void*)offset);
	glEnableVertexAttribArray(loc);
}

void prepareVAOStandard(vao& vao)
{
	vao.bind();
	vao.assignAttribute(0, offsetof(basic_vertex, x), 3, gltype::F32, sizeof(basic_vertex));
	vao.assignAttribute(1, offsetof(basic_vertex, u), 2, gltype::F32, sizeof(basic_vertex));
	vao.assignAttribute(2, offsetof(basic_vertex, nx), 3, gltype::F32, sizeof(basic_vertex));
}