#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "rendering/glm.h"

#include "lib/basic.h"
#include "lib/string.h"

#include "rendering/ogl/opengl.h"

#include "debug/log/Log.h"

class shader
{
public:
	shader(const string& vpath, const string& fpath);
	shader(const string& cpath);

	~shader();

	void uniform(const string& name, i32 val)
	{
		int location = uniformLocation(name);
		glUniform1i(location, val);
	}

	void uniform(const string& name, f32 val)
	{
		int location = uniformLocation(name);
		glUniform1f(location, val);
	}

	void uniform(const string& name, glm::vec2 val)
	{
		int location = uniformLocation(name);
		glUniform2f(location, val.x, val.y);
	}

	void uniform(const string& name, glm::vec3 val)
	{
		int location = uniformLocation(name);
		glUniform3f(location, val.x, val.y, val.z);
	}

	void uniform(const string& name, glm::vec4 val)
	{
		int location = uniformLocation(name);
		glUniform4f(location, val.x, val.y, val.z, val.w);
	}

	void uniform(const string& name, const glm::mat3& val, bool transpose = false)
	{
		int location = uniformLocation(name);
		glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(val));
	}

	void uniform(const string& name, const glm::mat4& val, bool transpose = false)
	{
		int location = uniformLocation(name);
		glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(val));
	}

	void bind()
	{
		glUseProgram(id);
	}

	glhandle getID() const
	{
		return id;
	}

private:
	glhandle id;

	int uniformLocation(const string& name)
	{
		return glGetUniformLocation(id, name.c_str());
	}
};