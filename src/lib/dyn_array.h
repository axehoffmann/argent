#pragma once

#include "lib/block_allocator.h"

/*
* Non-resizable, dynamically allocated array.
*/
template <typename T>
class dyn_array
{
public:
	T* begin() const
	{
		return data;
	}

	T* end() const
	{
		return data + size;
	}

	u32 size() const
	{
		return count;
	}

	dyn_array(u32 sz, block_allocator* alloc) :
		count(sz),
		allocator(alloc),
		data(static_cast<T*>(allocator->allocate(sizeof(T) * sz))) {}

	~dyn_array()
	{
		allocator->free(data);
	}

private:
	u32 count;
	block_allocator* allocator;
	T* data;

};