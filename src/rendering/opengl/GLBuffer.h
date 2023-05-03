#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>

#include "GL.h"

namespace ag
{
	enum class BufferType
	{
		VertexArray = GL_ARRAY_BUFFER,
		IndexArray = GL_ELEMENT_ARRAY_BUFFER,
		Storage = GL_SHADER_STORAGE_BUFFER,
		Uniform = GL_UNIFORM_BUFFER
	};

	enum class BufferAccessType
	{
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,

		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,

		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
	};

	class GLBuffer
	{
	public:
		GLBuffer(BufferType bufferType, BufferAccessType access, size_t bufferSize);
		~GLBuffer();

		template <typename T>
		void SetData(std::vector<T>& data)
		{
			size_t newSize = sizeof(T) * data.size();
			if (newSize > size)
			{
				/// TODO: throw warning about reallocating buffer memory
			}

			if (firstAlloc && staticBuffer)
			{
				/// TODO: throw warning about modifying static buffer data
			}

			firstAlloc = true;

			size = newSize;

			glBindBuffer(type, handle);
			glBufferData(type, newSize, (void*)data.data(), accessType);
		}

		template <typename T>
		void SetSubData(std::vector<T>& data, size_t offset)
		{
			size_t subSize = sizeof(T) * data.size();
			if (subSize + offset > size)
			{
				/// TODO: throw error about allocating outside buffer memory
			}

			if (staticBuffer)
			{
				/// TODO: throw error about modifying static buffer data
			}

			glBindBuffer(type, handle);
			glBufferSubData(type, offset, size, data.data());
		}

		GLHandle handle;
	private:
		size_t size;
		BufferType type;
		BufferAccessType accessType;

		bool firstAlloc;
		bool staticBuffer;
	};
}