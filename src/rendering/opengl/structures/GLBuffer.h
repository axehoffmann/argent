#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>

#include "rendering/opengl/GL.h"

#include "debug/log/Log.h"

namespace ag
{
	enum class BufferType
	{
		VertexData = GL_ARRAY_BUFFER,
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
		GLBuffer(BufferType bufferType, BufferAccessType access);
		GLBuffer(BufferType bufferType, BufferAccessType access, size_t bufferSize);
		GLBuffer(GLBuffer&& other) { other.handle = 0; }
		~GLBuffer();

		/**
		* Populates the buffer with the data vector.
		* @param data The vector of input data.
		*/
		template <typename T>
		void SetData(const std::vector<T>& data)
		{
			size_t newSize = sizeof(T) * data.size();
			if (newSize > size && firstAlloc)
			{
				Log::Warn("Reallocating GL Buffer memory, may be slow");
			}

			if (firstAlloc && staticBuffer)
			{
				Log::Warn("Modifying 'static' GL Buffer memory, may be slow");

			}

			firstAlloc = true;

			size = newSize;

			glBindBuffer((GLenum)type, handle);
			glBufferData((GLenum)type, newSize, (void*)data.data(), (GLenum)accessType);
		}

		/**
		* Assigns a subset of the buffer's data.
		* @param data The new subset of data.
		* @param offset The index into the buffer that the subset begins at.
		*/
		template <typename T>
		void SetSubData(const std::vector<T>& data, size_t offset)
		{
			size_t subSize = sizeof(T) * data.size();
			if (subSize + offset > size)
			{
				Log::Error("Reallocating buffer memory. Slow operation.");
			}

			if (staticBuffer)
			{
				Log::Error("Updating static buffer data. Slow operation.");
			}

			glBindBuffer((GLenum)type, handle);
			glBufferSubData((GLenum)type, sizeof(T) * offset, size, data.data());
		}

		void Bind()
		{
			glBindBuffer((GLenum)type, handle);
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