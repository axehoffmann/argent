#include "tests/test_framework.h"

#include "lib/mt/mt_list.h"

namespace mt_list_test
{
	$utest(mtlist);

	$tcase(synchronous, mtlist)
	{
		constexpr u64 count = 1000;

		block_allocator alloc(10);
		mt_list<u32> list(&alloc);

		for (u32 i = 0; i < 5; i++)
		{
			std::vector<u32*> ptrs(count);

			for (u32 i = 0; i < count; i++)
			{
				u32* p = list.push(u32{i});
				ptrs.at(i) = p;
			}

			assert_equals(list.size(), count);

			for (u64 i = 0; i < count; i++)
			{
				assert_equals(u64{*ptrs.at(i)}, i);
			}

			alloc.reset();
		}
	}
}