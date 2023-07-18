#pragma once

#include "GL.h"

namespace ag
{
	class GLVAO
	{
	public:
		GLVAO()
		{
			glGenVertexArrays(1, &handle);
		}

		void Bind()
		{
			glBindVertexArray(handle);
		}

		~GLVAO()
		{
			glDeleteVertexArrays(1, &handle);
		}
	private:
		GLHandle handle;
	};
}