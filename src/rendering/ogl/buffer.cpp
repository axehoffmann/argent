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

void buffer::setData(void* data, u64 sz)
{
	if (size == 0)
	{
		size = sz;
	}

	glBindBuffer(static_cast<GLenum>(storageType), handle);
	glBufferData(static_cast<GLenum>(storageType), size, data, static_cast<GLenum>(accessType));
}

void buffer::bind()
{
	glBindBuffer((GLenum)storageType, handle);
}
