#pragma once

#include <bit>
#include <memory>

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
	 * @return a pointer to the appended item. Note that its lifetime will end when reset() is called on the list.
	*/
	T* push(T&& ob)
	{
		count++;

		while (true)
		{
			// Try append to the block
			block* b = last.load();
			u64 offset = b->offset.fetch_add(size);

			if (offset > b->size)
			{
				// Another thread is allocating a new block right now, wait for them
				continue;
			}
			else if (offset + size > b->size)
			{
				// Our responsibility to create a new block
				b = newBlock();
				offset = b->offset.fetch_add(size);
			}

			T* loc = b->data + offset;
			*loc = std::move(ob);
			return loc;
		}
	}

	/**
	 * Completely resets the list
	*/
	void reset()
	{
		clear();

		block* nb = newBlock();
		first.store(nb);
		last.store(nb);
		count.store(0);
	}

	/**
	 * Copies all the data stored in the list to a destination.
	 * Expects that there is enough memory allocated contiguously at the destination.
	 * Should not be used while being written to.
	 * @param dest destination pointer
	*/
	void copy_to(T* dest) const
	{
		block* b = first.load();
		while (b != nullptr)
		{
			u64 amt = b->offset.load();
			std::copy(b->data, b->data + amt, dest);
			dest += amt;

			b = b->next;
		}
	}

	u64 size() const
	{
		return count;
	}

	~mt_list()
	{
		clear();
	}

private:	
	void clear()
	{
		block* b = first.load();
		while (b != nullptr)
		{
			b->~block(allocator);
			allocator->free(b);
		}
	}

	block* newBlock()
	{
		block* b = last.load();

		block* nb = allocator->allocate(sizeof(block));
		new (nb) block(b->size * 2);

		b->next.store(nb);
		last.store(nb);
	}

	struct block
	{
		u64 size;
		T* data;
		atomic<u64> offset;
		atomic<block*> next;

		block(u64 sz, block_allocator* alloc) :
			size(sz),
			data(new (alloc->allocate(sizeof(T) * size)) T[size]),
			offset(0),
			next(nullptr) {}

		~block(block_allocator* alloc)
		{
			alloc->free(data);
		}
	};
	
	atomic<block*> first;
	atomic<block*> last;
	
	atomic<u64> count;

	block_allocator* allocator;
};