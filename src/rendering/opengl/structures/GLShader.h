#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <string>

#include "rendering/opengl/GL.h"

#include "resources/Shader.h"
#include "debug/log/Log.h"

#include <string>


namespace ag
{
	class GLShader
	{
	public:
		/// TODO: build a GLShader from a shader resource
		GLShader(std::string vpath, std::string fpath);
		~GLShader();

		static GLShader FromResource(uint32_t resourceID)
		{
			std::weak_ptr<ag::Shader> wresource = ag::AssetManager::Fetch<ag::Shader>(resourceID);
			if (std::shared_ptr<ag::Shader> resource = wresource.lock())
			{
				return GLShader(resource->vertexPath, resource->fragmentPath);
			}

			Log::Error(sfmt("Couldn't get Shader resource with ID {} when trying to build GLShader", resourceID));
			throw std::runtime_error("Couldn't get Shader resource");
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

		void Bind()
		{
			glUseProgram(handle);
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