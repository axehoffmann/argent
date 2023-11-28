#include "tests/TestFramework.h"

#include "lib/block_allocator.h"

#include <algorithm>
#include <array>
#include <execution>
#include <vector>

namespace block_allocator_test
{
	$UTest(block_allocators);

	$Case(synchronous, block_allocators)
	{
		constexpr u32 alloc_count = 1000;

		block_allocator alloc(10);

		for (u32 i = 0; i < 5; i++)
		{
			std::vector<u32*> ptrs(alloc_count);

			for (u32 i = 0; i < alloc_count; i++)
			{
				u32* p = static_cast<u32*>(alloc.allocate(sizeof(u32)));
				new (p) u32{i};
				ptrs.at(i) = p;
			}

			for (u32 i = 0; i < alloc_count; i++)
			{
				ag_expect(*ptrs.at(i) == i, "");
				alloc.free(nullptr);
			}

			alloc.reset();
		}
	}

	$Case(parrallel, block_allocators)
	{
		constexpr u32 alloc_count = 1000;

		block_allocator alloc(10);

		std::array<u32, alloc_count> indices;
		std::iota(std::begin(indices), std::end(indices), 0);

		for (u32 i = 0; i < 5; i++)
		{
			std::array<u32*, alloc_count> ptrs;

			std::for_each(std::execution::par_unseq, std::begin(indices), std::end(indices), [&](u32 i)
			{
				u32* p = static_cast<u32*>(alloc.allocate(sizeof(u32)));
				new (p) u32{ i };
				ptrs.at(i) = p;
			});
			

			for (u32 i = 0; i < alloc_count; i++)
			{
				ag_expect(*ptrs.at(i) == i, "");
				alloc.free(nullptr);
			}

			alloc.reset();
		}
	}
}