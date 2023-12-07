#pragma once

#include "rendering/glm.h"

#include "ogl/vao.h"

#include <unordered_map>

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