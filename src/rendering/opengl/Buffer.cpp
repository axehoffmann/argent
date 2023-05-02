#include "Buffer.h"

ag::Buffer::Buffer(BufferType bufferType, BufferAccessType access, size_t bufferSize)
{
	glGenBuffers(1, &handle);
	type = bufferType;
	size = bufferSize;
	accessType = access;
	firstAlloc = false;

	staticBuffer = (accessType == BufferAccessType::StaticCopy 
					|| accessType == BufferAccessType::StaticDraw 
					|| accessType == BufferAccessType::StaticRead);
		
}

ag::Buffer::~Buffer()
{
	glDeleteBuffers(1, &handle);
}
