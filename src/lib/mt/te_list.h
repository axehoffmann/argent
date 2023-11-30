#pragma once

#include <bit>
#include <memory>

#include "lib/basic.h"
#include "lib/block_allocator.h"
#include "lib/sync.h"

/**
 * A multi-writer, type-erased block array. Uses a pool allocator.
 * Use case:
 *	- Faster iteration due to contiguous blocks
 *  - Lock-free append operation
*/
class mtte_list
{
private:
	struct block;
public:
	/**
	 * Concurrently appends an item to the list.
	 * @param ob the item to append. MUST represent the same type used by the rest of the list.
	 * @return a pointer to the appended item. Note that its lifetime will end when reset() is called on the list.
	*/
	ag::byte* push(ag::byte* ob)
	{
		count++;

		while (true)
		{
			// Try append to the block
			block* b = last.load();
			u64 offset = b->offset.fetch_add(tsize);

			if (offset > b->size)
			{
				// Another thread is allocating a new block right now, wait for them
				continue;
			}
			else if (offset + tsize > b->size)
			{
				// Our responsibility to create a new block
				b = newBlock();
				offset = b->offset.fetch_add(tsize);
			}

			std::byte* loc = b->data + offset;
			std::memcpy(loc, ob, tsize);
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
	void copy_to(ag::byte* dest) const
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
	
	mtte_list(u32 item_size)
		: tsize(item_size)
	{
		reset();
	}

	~mtte_list()
	{
		clear();
	}

private:
	void clear()
	{
		block* b = first.load();
		while (b != nullptr)
		{
			allocator->free(b->data);
			b->~block();
			allocator->free(b);
		}
	}

	block* newBlock()
	{
		block* b = last.load();

		block* nb = static_cast<block*>(allocator->allocate(sizeof(block)));
		new (nb) block(tsize, b->size * 2, allocator);

		b->next.store(nb);
		last.store(nb);
	}

	struct block
	{
		u64 size;
		ag::byte* data;
		atomic<u64> offset;
		atomic<block*> next;

		block(u32 isz, u64 sz, block_allocator* alloc) :
			size(sz),
			data(new (alloc->allocate(isz * size)) ag::byte[isz * size]),
			offset(0),
			next(nullptr) {}
	};

	atomic<block*> first;
	atomic<block*> last;

	u32 tsize;

	atomic<u64> count;

	block_allocator* allocator;
};