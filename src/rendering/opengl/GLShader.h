#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <string>

#include "GL.h"

#include <string>


namespace ag
{
	class GLShader
	{
	public:
		/// TODO: build a GLShader from a shader resource
		GLShader(std::string vpath, std::string fpath);
		~GLShader();

		void InitialiseAttribute(std::string attribName, int size, GLEnum type, bool normalized, int stride, int offset)
		{
			int loc = glGetAttribLocation(handle, attribName.c_str());
			glVertexAttribPointer(loc, size, type, normalized, stride, (void*)offset);
			glEnableVertexAttribArray(loc);
		}

		template <typename T>
		void Uniform(std::string name, T val) { // TODO: throw error 
		}

		template <>
		void Uniform(std::string name, int val)
		{
			int location = UniformLocation(name);
			glUniform1i(location, val);
		}

		template <>
		void Uniform(std::string name, float val)
		{
			int location = UniformLocation(name);
			glUniform1f(location, val);
		}

		template <>
		void Uniform(std::string name, const glm::vec3& val)
		{
			int location = UniformLocation(name);
			glUniform3f(location, val.x, val.y, val.z);
		}

		template <>
		void Uniform(std::string name, const glm::vec4& val)
		{
			int location = UniformLocation(name);
			glUniform4f(location, val.x, val.y, val.z, val.w);
		}


		template <typename T>
		void Uniform(std::string name, const T& val, bool transpose) { /// TODO: throw error 
		}

		template <>
		void Uniform(std::string name, const glm::mat3& val, bool transpose)
		{
			int location = UniformLocation(name);
			glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(val));
		}

		template <>
		void Uniform(std::string name, const glm::mat4& val, bool transpose)
		{
			int location = UniformLocation(name);
			glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(val));
		}





		GLHandle handle;
	private:

		std::string ReadText(std::string path, std::string name);

		void CompileShader(std::string source, GLHandle dest);

		int UniformLocation(std::string name)
		{
			return glGetUniformLocation(handle, name.c_str());
		}
		// std::vector<std::string>> uniforms
	};
}