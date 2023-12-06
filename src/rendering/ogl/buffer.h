#pragma once

#include "opengl.h"

enum class buffer_type : GLenum
{
	VertexData = GL_ARRAY_BUFFER,
	IndexArray = GL_ELEMENT_ARRAY_BUFFER,
	Storage = GL_SHADER_STORAGE_BUFFER,
	Uniform = GL_UNIFORM_BUFFER
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

	void setData(void* data, u64 size, u64 offset = 0);

	void bind();

private:
	buffer_type storageType;
	buffer_access_type accessType;
	glhandle handle;
	u64 size;
};