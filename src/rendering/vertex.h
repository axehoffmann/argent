#pragma once

#include "ogl/vao.h"

#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct vertex
{
	glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

	static void prepareVAO(vao& vao)
	{
		vao.bind();
		vao.assignAttribute(0, offsetof(vertex, position), 3, gltype::F32, sizeof(vertex));
		vao.assignAttribute(1, offsetof(vertex, uv),	   2, gltype::F32, sizeof(vertex));
		vao.assignAttribute(2, offsetof(vertex, normal),   3, gltype::F32, sizeof(vertex));
	}
};