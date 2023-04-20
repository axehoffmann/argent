#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

typedef unsigned int GLHandle;
typedef unsigned int GLEnum;

namespace ag
{
	class GL
	{
	public:

		/// TODO: Error checking with invalid buffers etc?
		static void BindBuffer(GLEnum type, GLHandle handle)
		{
			glBindBuffer(type, handle);
		}

		static void BindTexture(GLEnum type, int slot, GLHandle handle)
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(type, handle);
		}

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

		/// TODO: check if the variable is valid in shader metadata maybe?
		static void SetInt(std::string name, int value, GLHandle shader)
		{
			int location = glGetUniformLocation(shader, name.c_str());
			glUniform1i(location, value);
		}

		static void SetFloat(std::string name, float value, GLHandle shader)
		{
			int location = glGetUniformLocation(shader, name.c_str());
			glUniform1f(location, value);
		}

		static void SetMat3(std::string name, const glm::mat3& value, GLHandle shader, bool transpose = false)
		{
			int location = glGetUniformLocation(shader, name.c_str());
			glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
		}

		static void SetMat4(std::string name, const glm::mat4& value, GLHandle shader, bool transpose = false)
		{
			int location = glGetUniformLocation(shader, name.c_str());
			glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
		}

		static void SetVec3(std::string name, const glm::vec3& value, GLHandle shader)
		{
			int location = glGetUniformLocation(shader, name.c_str());
			glUniform3f(location, value.x, value.y, value.z);
		}

		static void SetVec4(std::string name, const glm::vec4& value, GLHandle shader)
		{
			int location = glGetUniformLocation(shader, name.c_str());
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}

		static void DrawIndexed(GLEnum primitiveType, size_t count, GLEnum indexType, size_t offset)
		{
			glDrawElements(primitiveType, count, indexType, (void*)offset);
		}
	};
}