#include "shader.h"

#include <iostream>

#include <sstream>
#include <fstream>

string readText(const string& path)
{
	string source;
	std::ifstream stream(path);
	if (stream.is_open())
	{
		std::stringstream s;
		s << stream.rdbuf();
		source = s.str();
		stream.close();
	}
	else
		throw std::runtime_error("Failed to open " + path);

	return source;
}

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

	handle = glCreateProgram();

	glAttachShader(handle, vert);
	glAttachShader(handle, frag);
	glLinkProgram(handle);

	int success = GL_FALSE;
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		int logLength = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorLog(logLength + 1);
		glGetProgramInfoLog(handle, 512, NULL, errorLog.data());
		ag::Log::Error(ag::sfmt("failed to link shader program : \n{} ", errorLog.data()));
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

shader::~shader()
{
	glDeleteProgram(handle);
}