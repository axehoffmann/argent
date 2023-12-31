#include "buffer.h"

#include "debug/log/Log.h"

buffer::buffer(buffer_access_type access, buffer_type type) :
	storageType(type),
	accessType(access),
	id(-1),
	size(0)
{
	glCreateBuffers(1, &id);
}

buffer::buffer(buffer&& other) noexcept :
	storageType(other.storageType),
	accessType(other.accessType),
	id(other.id),
	size(other.size)
{
	other.id = -1;
}

buffer& buffer::operator=(buffer&& other) noexcept
{
	storageType = other.storageType;
	accessType = other.accessType;
	id = other.id;
	size = other.size;
	other.id = -1;

	return *this;
}

buffer::~buffer()
{
	if (id == -1)
		return;

	glDeleteBuffers(1, &id);
}

void buffer::allocate(u64 sz)
{
	size = sz;
	glNamedBufferStorage(id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void buffer::set(void* data, u64 sz, u64 offset)
{
	glNamedBufferSubData(id, offset, sz, data);
}

void buffer::setData(void* data, u64 sz)
{
	size = sz;
	glNamedBufferData(id, size, data, static_cast<GLenum>(accessType));
}

void buffer::bind()
{
	glBindBuffer(static_cast<GLenum>(storageType), id);
}
