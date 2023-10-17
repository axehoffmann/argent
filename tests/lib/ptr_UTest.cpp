#include "../TestFramework.h"

#include "lib/ptr.h"

namespace ptr_test
{
	struct TestObject
	{
		TestObject(int* tr) : target(tr)
		{
			*target = 1;
		}
		~TestObject()
		{
			*target = 2;
		}
	private:
		int* target;
	};

	$Test(ptr_test);

	$Case(construction_destruction, ptr_test)
	{
		int test = 0;
	
		{
			ptr<TestObject> ob = make<TestObject>(&test);

			ag_expect(test == 1, "Expected ptr to initialise the object");
		}

		ag_expect(test == 2, "Expected ptr to destruct the object");
	}
}