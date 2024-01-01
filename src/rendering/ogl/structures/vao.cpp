#pragma once

#include "vao.h"
#include "resources/v2/packer/mesh/mesh.h"

vao::vao() :
	id(-1)
{
	glGenVertexArrays(1, &id);
}

vao::~vao()
{
	if (id == -1)
		return;

	glDeleteVertexArrays(1, &id);
}

vao::vao(vao&& other) noexcept :
	id(other.id)
{
	other.id = -1;
}

vao& vao::operator=(vao&& other) noexcept
{
	id = other.id;
	other.id = -1;
	return *this;
}

void vao::bind()
{
	glBindVertexArray(id);
}

void vao::link(buffer& vbo, buffer& ebo, u32 stride)
{
	glVertexArrayVertexBuffer(id, 0, vbo.getID(), 0, stride);
	glVertexArrayElementBuffer(id, ebo.getID());
}

void vao::assignAttribute(u32 loc, u64 offset, i32 size, gltype type, bool normalised)
{
	glEnableVertexArrayAttrib(id, loc);
	glVertexArrayAttribFormat(id, loc, size, static_cast<GLenum>(type), normalised, offset);
	glVertexArrayAttribBinding(id, loc, 0);
}

void prepareVAOStandard(vao& vao, buffer& vbo, buffer& ebo)
{
	vao.link(vbo, ebo, sizeof(basic_vertex));
	vao.assignAttribute(0, offsetof(basic_vertex, x), 3, gltype::F32);
	vao.assignAttribute(1, offsetof(basic_vertex, u), 2, gltype::F32);
	vao.assignAttribute(2, offsetof(basic_vertex, nx), 3, gltype::F32);
	vao.assignAttribute(3, offsetof(basic_vertex, tx), 3, gltype::F32);
}