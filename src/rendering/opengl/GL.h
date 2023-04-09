#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

typedef unsigned int GLHandle;
typedef unsigned int GLEnum;

namespace ag
{
	class GL
	{
	public:

		static GLHandle MakeBuffer(GLEnum type, size_t size, void* data, GLEnum hint)
		{
			GLHandle buf;

			glGenBuffers(1, &buf);
			glBindBuffer(type, buf);
			glBufferData(type, size, data, hint);

			return buf;
		}
	};
}