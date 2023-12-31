#pragma once

#include "opengl.h"
#include "buffer.h"

#include "resources/v2/packer/mesh.h"

class vao
{
public:
	vao();
	~vao();

	vao(vao&) = delete;
	vao& operator=(vao&) = delete;

	vao(vao&& other) noexcept;
	vao& operator=(vao&& other) noexcept;

	void bind();

	void link(buffer& vbo, buffer& ebo, u32 stride);

	void assignAttribute(u32 loc, u64 offset, i32 size, gltype type, bool normalised = false);

private:
	glhandle id;
};

void prepareVAOStandard(vao& vao, buffer& vbo, buffer& ebo);