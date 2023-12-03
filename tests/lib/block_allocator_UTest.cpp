#include "tests/test_framework.h"

#include "lib/block_allocator.h"

#include <algorithm>
#include <array>
#include <execution>
#include <vector>

namespace block_allocator_test
{
	$utest(block_allocators);

	$tcase(synchronous, block_allocators) 
	{
		constexpr u32 alloc_count = 1000;

		block_allocator alloc(10);

		for (u32 rep = 0; rep < 5; rep++)
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
				assert_equals(*ptrs.at(i), i);
				alloc.free(nullptr);
			}

			alloc.reset();
		}
	}

	$tcase(parallel, block_allocators)
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
				assert_equals(*ptrs.at(i), i);
				alloc.free(nullptr);
			}

			alloc.reset();
		}
	}
}