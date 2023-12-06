#include "buffer.h"

#include "debug/log/Log.h"

buffer::buffer(buffer_access_type access, buffer_type type) :
	storageType(type),
	accessType(access),
	handle(-1),
	size(0)
{
	glGenBuffers(1, &handle);
}

buffer::buffer(buffer&& other) noexcept :
	storageType(other.storageType),
	accessType(other.accessType),
	handle(other.handle),
	size(other.size)
{
	other.handle = -1;
}

buffer& buffer::operator=(buffer&& other) noexcept
{
	storageType = other.storageType;
	accessType = other.accessType;
	handle = other.handle;
	size = other.size;
	other.handle = -1;

	return *this;
}

buffer::~buffer()
{
	if (handle == -1)
		return;

	glDeleteBuffers(1, &handle);
}

void buffer::setData(void* data, u64 sz, u64 offset)
{
	if (size == 0)
	{
		size = sz;
	}
	else if (sz + offset > size)
	{
		ag::Log::Error("Reallocating a GL buffer");
	}

	glBindBuffer(static_cast<GLenum>(storageType), handle);
	glBufferSubData(static_cast<GLenum>(storageType), offset, size, data);
}

void buffer::bind()
{
	glBindBuffer((GLenum)storageType, handle);
}
