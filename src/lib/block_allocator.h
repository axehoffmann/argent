#pragma once

#include <bit>
#include <cassert>
#include <cstring>
#include <memory>

#include "debug/log/Log.h"

#include "basic.h"
#include "sync.h"

/*
* Allocates in incremental blocks, and is synchronised.
* Resetting wipes all blocks.
*/
class block_allocator
{
private:
	struct block;

public:
	/**
	 * Allocate memory in the block allocator
	 * @param size 
	 * @return a pointer to the beginning of the allocated memory
	*/
	void* allocate(u64 size)
	{
		counter++;

		while (true)
		{
			// Try append to the block
			block* b = last.load();
			u64 offset = b->end.fetch_add(size);

			if (offset > b->size)
			{
				// Another thread is allocating a new block right now, wait for them
				continue;
			}
			else if (offset + size >= b->size)
			{
				// Our responsibility to create a new block
				b = newBlock();
				offset = b->end.fetch_add(size);
			}

			return (b->data + offset);
		}
	}

	/**
	 * Call after destructing an allocation
	*/
	void free(void* loc)
	{
		counter.fetch_add(-1);
	}

	/**
	 * Wipes the allocator clean. Assumes that all data has been properly destructed. Not a synchronised operation.
	*/
	void reset()
	{
		if (counter != 0)
		{
			ag::Log::Error("Panic! Pool allocator was not properly cleaned up before reset");
		}

		delete first;
		// Reset with our highest total size, to avoid more reallocations
		first = new block(std::bit_ceil(totalSize.load()));
		last.store(first);
	}

	block_allocator(u64 initial_size) :
		counter(0),
		totalSize(initial_size),
		allocating(),
		first(new block(initial_size)),
		last(first) {}

	atomic<u32> heap_allocs;


private:

	block* newBlock()
	{
		if (allocating.test_and_set())
		{
			ag::Log::Error("Panic! Pool Allocator is trying to allocate twice at once.");
		}

		block* cur = last.load();

		block* nb = new block(cur->size * 2);
		heap_allocs.fetch_add(1);
		totalSize.fetch_add(nb->size);
		cur->next.store(nb);
		last.store(nb);

		allocating.clear();

		return nb;
	}

	struct block
	{
		block(u64 sz) :
			size(sz),
			data(new ag::byte[size]),
			end(0),
			next(nullptr) {}

		~block()
		{
			delete[] data;
			delete next;
		}

		u64 size;
		ag::byte* data;
		atomic<u64> end;
		atomic<block*> next;
	};

	// Counter to ensure everything has been safely destructed before resetting
	atomic<u64> counter;

	atomic<u64> totalSize;

	std::atomic_flag allocating;

	block* first;
	atomic<block*> last;
};