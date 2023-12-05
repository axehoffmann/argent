#pragma once

#include <bit>
#include <memory>

#include "lib/basic.h"
#include "lib/block_allocator.h"
#include "lib/sync.h"

/**
 * A two-phase multi-writer block array. Uses a pool allocator.
 * Note that the list should not be iterated/read while being written to.
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
		bool first = count.fetch_add(1) == 0;

		while (true)
		{
			// Try append to the block
			block* b = last.load();

			/// TODO: tidy up?
			if (first && b == nullptr)
			{
				prepare();
				b = last.load();
			}
			else if (b == nullptr)
			{
				continue;
			}

			u64 offset = b->offset.fetch_add(1);

			if (offset > b->size)
			{
				// Another thread is allocating a new block right now, wait for them
				continue;
			}
			else if (offset + 1 >= b->size)
			{
				// Our responsibility to create a new block
				b = newBlock();
				offset = b->offset.fetch_add(1);
			}

			T* loc = b->data + offset;
			*loc = std::move(ob);
			return loc;
		}
	}

	/**
	 * Completely wipes the list, freeing memory.
	*/
	void reset()
	{
		clear();
		count.store(0);
		first.store(nullptr);
		last.store(nullptr);
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
			if (amt > b->size) amt = b->size;

			std::copy(b->data, b->data + amt, dest);
			dest += amt - 1;

			b = b->next;
		}
	}

	u64 size() const
	{
		return count;
	}

	mt_list(u32 sz, block_allocator* a) : 
		first(static_cast<block*>(a->allocate(sizeof(block)))),
		last(),
		count(0),
		allocator(a),
		totalSize(sz)
	{
		last.store(first.load());
		new (first) block(sz, allocator);
	}

	~mt_list()
	{
		clear();
	}

	struct iterator
	{
		bool operator==(const iterator& other) const
		{
			return other.current == current
				&& other.currentIdx == currentIdx;
		}

		iterator& operator++()
		{
			increment();
			return *this;
		}

		iterator operator++(i32)
		{
			iterator tmp = *this;
			increment();
			return tmp;
		}

		T& operator*() const
		{
			return *(current->data + currentIdx);
		}

	private:
		void increment()
		{
			currentIdx++;
			if (currentIdx >= current->offset.load())
			{
				current = current->next;
				currentIdx = 0;
			}
		}

		block* current;
		u64 currentIdx;
	};

	iterator begin() const
	{
		return iterator { .current = first.load(), .currentIdx = 0 };
	}

	iterator end() const
	{
		return iterator { .current = nullptr, .currentIdx = 0 };
	}

private:	
	void prepare()
	{
		block* nb = static_cast<block*>(allocator->allocate(sizeof(block)));
		new (nb) block(totalSize, allocator);

		first.store(nb);
		last.store(nb);
	}

	void clear()
	{
		block* b = first.load();
		while (b != nullptr)
		{
			block* cur = b;
			b = cur->next;

			allocator->free(cur->data);
			cur->~block();
			allocator->free(cur);
		}
	}

	block* newBlock()
	{
		block* b = last.load();

		block* nb = static_cast<block*>(allocator->allocate(sizeof(block)));
		new (nb) block(b->size * 2, allocator);
		totalSize += b->size * 2;

		b->next.store(nb);
		last.store(nb);

		return nb;
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
	};
	
	atomic<block*> first;
	atomic<block*> last;
	
	atomic<u64> count;
	atomic<u64> totalSize;

	block_allocator* allocator;
};