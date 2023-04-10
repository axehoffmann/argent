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

		static GLHandle MakeTexture(GLEnum type, GLEnum format, int width, int height, u_char* data, bool mipmap = true)
		{
			GLHandle tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			if (mipmap)
				glGenerateMipmap(GL_TEXTURE_2D);
			
			return tex;
		}
	};
}