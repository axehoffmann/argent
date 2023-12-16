#pragma once

#include "opengl.h"

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

	void assignAttribute(u32 loc, u64 offset, i32 size, gltype type, i32 stride, bool normalised = false);

private:
	glhandle handle;
};

void prepareVAOStandard(vao& vao);