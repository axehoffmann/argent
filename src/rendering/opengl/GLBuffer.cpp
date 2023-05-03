#include "GLBuffer.h"

ag::GLBuffer::GLBuffer(BufferType bufferType, BufferAccessType access, size_t bufferSize)
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

ag::GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &handle);
}
