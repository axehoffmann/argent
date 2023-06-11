#pragma once

typedef unsigned int GLEnum;
typedef unsigned int GLHandle;

namespace ag
{
	class GL
	{
		static void DrawIndexed(GLEnum primitiveType, size_t count, GLEnum indexType, size_t offset)
		{
			glDrawElements(primitiveType, count, indexType, (void*)offset);
		}
	};
}