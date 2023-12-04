#include "tests/test_framework.h"

#include "lib/mt/mt_list.h"

#include <algorithm>
#include <array>
#include <execution>
#include <numeric>

namespace mt_list_test
{
	$utest(mtlist);

	$tcase(synchronous, mtlist)
	{
		constexpr u64 count = 1000;

		block_allocator alloc(2048);

		mt_list<u32> list(16, &alloc);

		// Repeat to ensure validity after resets
		for (u32 rep = 0; rep < 5; rep++)
		{
			std::vector<u32*> ptrs(count);

			for (u32 i = 0; i < count; i++)
			{
				ptrs.at(i) = list.push(u32{ i });
			}

			assert_equals(list.size(), count);

			// Validate the output of the list
			for (u64 i = 0; i < count; i++)
			{
				assert_equals(u64{*ptrs.at(i)}, i);
			}

			list.reset();
			alloc.reset();
		}
	}

	$tcase(parallel, mtlist)
	{
		/// TODO: seems to cause UB sometimes on larger numbers
		constexpr u64 count = 1000;

		block_allocator alloc(2048);

		mt_list<u32> list(16, &alloc);

		std::array<u32, count> indices;
		std::iota(std::begin(indices), std::end(indices), 0);

		// Repeat to ensure validity after resets
		for (u32 rep = 0; rep < 5; rep++)
		{
			std::array<u32*, count> ptrs;

			// Push to the list in parallel
			std::for_each(std::execution::par_unseq, std::begin(indices), std::end(indices), [&](u32 i)
			{
				ptrs.at(i) = list.push(u32{i});
			});

			// Validate the output of the list
			for (u64 i = 0; i < count; i++)
			{
				assert_equals(u64{ *ptrs.at(i) }, i);
			}

			list.reset();
			alloc.reset();
		}
	}
}