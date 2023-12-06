#pragma once

#include "opengl.h"

class vao
{
public:
	vao();
	~vao();

	vao(vao&) = delete;
	vao& operator=(vao&) = delete;

	vao(vao&& other);
	vao& operator=(vao&& other) noexcept;

	void bind();

	void assignAttribute(u32 loc, u64 offset, i32 size, gltype type, i32 stride, bool normalised = false);

private:
	glhandle handle;
	static inline glhandle bound = -1;

};