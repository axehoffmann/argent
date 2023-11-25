#pragma once

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
	 * @return 
	*/
	void* allocate(u64 size)
	{
		block* cur = waitForBlock();

		u64 offset = cur->end.fetch_add(size);
		// If we are the first to allocate over the block's size, its our responsibility to create a new block
		if (offset + size > cur->size)
		{
			cur = newBlock();
		}
		/// TODO: analyse for off-by-one errors?
		else if (offset > cur->size)
		{
			cur = waitForBlock();
		}

		/// TODO: may need to theoretically loop over it.
		offset = cur->end.fetch_add(size);
		void* out = new (cur->data + offset);
		counter.fetch_add(1);
		return out;
	}

	/**
	 * Call after destructing an allocation
	*/
	void free()
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
		first = new block(totalSize);
		last.store(first);
	}

private:
	
	block* waitForBlock()
	{
		while (true)
		{
			block* cur = last.load();
			if (cur->end < cur->size) return cur;
		}
	}

	block* newBlock()
	{
		if (allocating.test_and_set(true))
		{
			ag::Log::Error("Panic! Pool Allocator is trying to allocate twice at once.");
		}

		block* cur = last.load();

		block* nb = new block(cur->size * 2);
		totalSize.fetch_add(nb->size);
		cur->next.store(nb);
		last.store(nb);
		allocating = false;
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