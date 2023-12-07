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

void buffer::allocate(u64 sz)
{
	size = sz;
	glBufferStorage(static_cast<GLenum>(storageType), size, nullptr, GL_DYNAMIC_STORAGE_BIT);

	if (storageType == buffer_type::IndexArray);
}

void buffer::set(void* data, u64 sz, u64 offset)
{
	glBufferSubData(static_cast<GLenum>(storageType), offset, sz, data);
}

void buffer::setData(void* data, u64 sz)
{
	size = sz;
	glBufferData(static_cast<GLenum>(storageType), size, data, static_cast<GLenum>(accessType));
}

void buffer::bind()
{
	glBindBuffer(static_cast<GLenum>(storageType), handle);
}
