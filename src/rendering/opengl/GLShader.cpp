#include "GLShader.h"

#include <iostream>

#include <sstream>
#include <fstream>

ag::GLShader::GLShader(std::string vpath, std::string fpath)
{
	GLHandle vert = glCreateShader(GL_VERTEX_SHADER);
	GLHandle frag = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vSource = ReadText(vpath, "vertex shader");
	std::string fSource = ReadText(fpath, "fragment shader");

	CompileShader(vSource, vert);
	CompileShader(fSource, frag);

	handle = glCreateProgram();

	glAttachShader(handle, vert);
	glAttachShader(handle, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

ag::GLShader::~GLShader()
{
	glDeleteProgram(handle);
}

std::string ag::GLShader::ReadText(std::string path, std::string name)
{
	std::string source;
	std::ifstream stream(path);
	if (stream.is_open())
	{
		std::stringstream s;
		s << stream.rdbuf();
		source = s.str();
		stream.close();
	}
	else
		throw new std::runtime_error("Failed to open " + name + " at " + path);

	return source;
}

void ag::GLShader::CompileShader(std::string source, GLHandle dest)
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
		glGetShaderInfoLog(dest, logLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << VertexShaderErrorMessage.data() << std::endl;
	}
}