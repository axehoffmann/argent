#include "opengl.h"

void checkError(std::source_location loc)
{
	int err = glGetError();
	if (err)
		ag::Log::Error(ag::sfmt("OpenGL error: [{}]", err, (const char*)glewGetErrorString(err)), loc);
}