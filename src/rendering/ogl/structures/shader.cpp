#include "shader.h"

#include <iostream>

#include "lib/read_file.h"

void compileShader(const string& source, glhandle dest)
{
	GLint result = GL_FALSE;
	int logLength;
	const char* sourceStr = source.c_str();
	glShaderSource(dest, 1, &sourceStr, NULL);
	glCompileShader(dest);

	glGetShaderiv(dest, GL_COMPILE_STATUS, &result);
	glGetShaderiv(dest, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(logLength + 1);
		glGetShaderInfoLog(dest, logLength, NULL, VertexShaderErrorMessage.data());
		std::cout << VertexShaderErrorMessage.data() << std::endl;
	}
}

shader::shader(const string& vpath, const string& fpath)
{
	glhandle vert = glCreateShader(GL_VERTEX_SHADER);
	glhandle frag = glCreateShader(GL_FRAGMENT_SHADER);

	string vSource = readText(vpath);
	string fSource = readText(fpath);

	compileShader(vSource, vert);
	compileShader(fSource, frag);

	id = glCreateProgram();

	glAttachShader(id, vert);
	glAttachShader(id, frag);
	glLinkProgram(id);

	int success = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		int logLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorLog(logLength + 1);
		glGetProgramInfoLog(id, 512, NULL, errorLog.data());
		ag::Log::Error(ag::sfmt("failed to link shader program : \n{} ", errorLog.data()));
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

shader::shader(shader&& other) :
	id(other.id)
{
	other.id = 0;
}

shader::shader(const string& cpath)
{
	glhandle csh = glCreateShader(GL_COMPUTE_SHADER);

	string csrc = readText(cpath);

	compileShader(csrc, csh);

	id = glCreateProgram();

	glAttachShader(id, csh);
	glLinkProgram(id);

	int success = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		int logLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorLog(logLength + 1);
		glGetProgramInfoLog(id, 512, NULL, errorLog.data());
		ag::Log::Error(ag::sfmt("failed to link compute shader program : \n{} ", errorLog.data()));
	}

	glDeleteShader(csh);
}

shader::~shader()
{
	if (id != 0)
	{
		glDeleteProgram(id);
	}
}