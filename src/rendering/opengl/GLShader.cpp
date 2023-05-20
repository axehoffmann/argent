#include "GLShader.h"

ag::GLShader::GLShader()
{
}

ag::GLShader::~GLShader()
{
	glDeleteProgram(handle);
}
