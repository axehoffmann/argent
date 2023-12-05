#include "tests/test_framework.h"

#include "lib/block_allocator.h"
#include "lib/dyn_array.h"

namespace dyn_array_test
{
	$utest(dyn_arrays);

	$tcase(general, dyn_arrays)
	{
		block_allocator alloc(1024);


	}
}