#pragma once

#include "lib/basic.h"
#include "lib/vector.h"

namespace ag
{
	// A colony implementation using an external skiplist
	constexpr u8 colonyBlockSize = 128;

	struct colony
	{
		/**
		 * Batch appends data to the colony.
		 * @param data		A pointer to the contiguous data
		 * @param count		Amount of units in the data
		*/
		void append(range<byte*>&& data) 
		{
			block_te* block_it = blocks.back();
			byte* it = data.begin();

			/// TODO: straight up allocate large enough blocks straight away
			while (it != data + (unit_size * data.width()))
			{
				u64 remainder = u64{block_it->back} - u64{block_it->back};

				std::memcpy(block_it->back, it, remainder);
				it += remainder;

				if (block_it->back == block_it->end)
				{
					// Allocate a new block
					block_te* next = allocator();
					block_it->next_block = next;
					blocks.push_back(next);
					block_it = next;
				}
			}
		}

	private:
		using block_alloc_func = block_te(*)();

		template <typename T>
		friend colony createColony();
		friend class archetype;

		colony(block_alloc_func alloc, u8 sz) : allocator(alloc), unit_size(sz) {}

		struct block_te
		{
			byte* begin;
			byte* back;
			byte* end;
			block_te* next_block;

			block_te(byte* b, byte e) : begin(b), back(b), end(e), next_block(nullptr) {}

			virtual ~block_te() 
			{
				delete next_block;
			}
		};

		template <typename T>
		struct block : block_te
		{
			block() : block_te(&data, &data + sizeof(T) * colonyBlockSize) {}

			T data[colonyBlockSize];
		};

		block_alloc_func allocator;
		vector<block_te*> blocks;
		u8 unit_size;
	};

	template <typename T>
	colony createColony()
	{
		return colony([]() 
		{
			return static_cast<colony::block_te*>(new colony::block<T>())
		});
	}


	struct skip_field
	{
	private:
		struct block
		{
			u8 data[colonyBlockSize];
			block* next_block;
		};
	};
}