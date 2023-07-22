#pragma once

#include "rendering/opengl/GL.h"
#include "debug/log/Log.h"

namespace ag
{
	class GLVAO
	{
	public:
		GLVAO()
		{
			glGenVertexArrays(1, &handle);
			Log::Error("Vao");
		}

		void Bind()
		{
			glBindVertexArray(handle);
			bound = handle;
		}

		void InitialiseAttribute(int loc, int size, GLEnum type, bool normalized, int stride, int offset)
		{
			if (bound != handle)
				Bind();
			glVertexAttribPointer(loc, size, type, normalized, stride, (void*)offset);
			glEnableVertexAttribArray(loc);
		}

		~GLVAO()
		{
			glDeleteVertexArrays(1, &handle);
		}
	private:
		GLHandle handle;
		static GLHandle bound;
	};
}