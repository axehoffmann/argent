#pragma once

#include <memory>
#include <bit>

#include "lib/basic.h"
#include "lib/block_allocator.h"
#include "lib/sync.h"

/**
 * A multi-writer block array. Uses a pool allocator.
 * Use case:
 *	- Faster iteration due to contiguous blocks
 *  - Lock-free append operation
*/
template <typename T>
class mt_list
{
private:
	struct block;
public:
	/**
	 * Concurrently appends an item to the list.
	 * @param ob the item to append
	*/
	void push(T&& ob)
	{
		count++;

		while (true)
		{
			block* b = waitForBlock();

		}
	}

	/**
	 * Completely resets the list
	*/
	void reset()
	{
		block* cur = first.load();
		while (cur != nullptr)
		{
			cur->~block(allocator);
			allocator->free(cur);
		}
		block* nb = newBlock();
		first.store(nb);
		last.store(nb);
		count.store(0);
	}

	/**
	 * Copies all the data stored in the list to a destination.
	 * Expects that there is enough memory allocated contiguously at the destination.
	 * @tparam Iter 
	 * @param dest 
	*/
	template <typename Iter>
	void copy_to(Iter dest) const
	{
		
	}

	u64 size() const
	{
		return count;
	}

private:
	struct block
	{
		u64 size;
		T* data;
		atomic<u64> offset;
		atomic<block*> next;

		block(u64 sz, block_allocator* alloc) :
			size(sz),
			data(alloc->allocate(sizeof(T) * size)),
			offset(0),
			next(nullptr) {}

		~block(block_allocator* alloc)
		{
			alloc->free(data);
		}
	};

	block* waitForBlock()
	{
		while (true)
		{
			block* cur = last.load();
			if (cur->offset.load() < cur->size) return cur;
		}
	}
	
	atomic<block*> first;
	atomic<block*> last;
	
	atomic<u64> count;

	block_allocator* allocator;
};