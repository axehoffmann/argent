#pragma once

#include "ogl/vao.h"

#include <glm/vec3.hpp>

struct vertex
{
	glm::vec3 position;

	static void prepareVAO(vao& vao)
	{
		vao.bind();
		vao.assignAttribute(0, offsetof(vertex, position), 3, gltype::F32, sizeof(vertex));
	}
};