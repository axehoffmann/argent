#pragma once

#include "lib/basic.h"

#define GLEW_STATIC
#include <GL/glew.h>

using glhandle = u32;

enum class gltype : GLenum
{
	U8 = GL_UNSIGNED_BYTE,
	U16 = GL_UNSIGNED_SHORT,
	U32 = GL_UNSIGNED_INT,

	I8 = GL_BYTE,
	I16 = GL_SHORT,
	I32 = GL_INT,

	F16 = GL_HALF_FLOAT,
	F32 = GL_FLOAT,
	F64 = GL_DOUBLE,

	I32_2_10_10_10 = GL_INT_2_10_10_10_REV,
	U32_2_10_10_10 = GL_UNSIGNED_INT_2_10_10_10_REV,
	F32_10_11_10 = GL_UNSIGNED_INT_10F_11F_11F_REV
};