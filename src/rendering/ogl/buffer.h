#pragma once

#include "opengl.h"

enum class buffer_type : GLenum
{
	VertexData = GL_ARRAY_BUFFER,
	IndexArray = GL_ELEMENT_ARRAY_BUFFER,
	
	Storage = GL_SHADER_STORAGE_BUFFER,
	Uniform = GL_UNIFORM_BUFFER,

	Command = GL_DRAW_INDIRECT_BUFFER
};

enum class buffer_access_type : GLenum
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

class buffer
{
public:
	buffer(buffer_access_type access, buffer_type type);
	
	buffer(buffer&) = delete;
	buffer& operator=(buffer&) = delete;

	buffer(buffer&& other) noexcept;
	buffer& operator=(buffer&& other) noexcept;

	~buffer();

	/**
	 * Preallocates a GPU buffer with the specified size
	 * @param sz the size of the buffer (in bytes)
	*/
	void allocate(u64 sz);

	/**
	 * Sets a subset of the buffer's data, starting at offset
	 * @param data   the beginning of the new data
	 * @param sz	 the size of the new data
	 * @param offset the offset into the buffer
	*/
	void set(void* data, u64 sz, u64 offset);

	void setData(void* data, u64 sz);

	u64 bufSize() const
	{
		return size;
	}

	/**
	 * Binds the buffer to the slot designated by its type.
	 * Types that can bind multiple buffers should instead use void bind(u32).
	*/
	void bind();

	void bindAs(buffer_type type)
	{
		glBindBuffer(static_cast<GLenum>(type), id);
	}

	void bind(u32 binding)
	{
		bind();
		glBindBufferBase(static_cast<GLenum>(storageType), binding, id);
	}	

	void copyTo(void* dest, u64 sz)
	{
		glGetNamedBufferSubData(id, 0, sz, dest);
	}

	glhandle getID() const
	{
		return id;
	}

private:
	buffer_type storageType;
	buffer_access_type accessType;
	glhandle id;
	u64 size;
};