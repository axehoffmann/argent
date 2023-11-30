#include "tests/test_framework.h"

#include "lib/ptr.h"

namespace ptr_test
{
	struct TestObject
	{
		TestObject(int* tr) : target(tr)
		{
			*target = 1;
		}
		virtual ~TestObject()
		{
			*target = 2;
		}
	private:
		int* target;
	};

	struct TestSubObject : public TestObject
	{
		TestSubObject(int* tr) : TestObject(tr) {}
		~TestSubObject() override {}
	};

	$utest(ptr_test);

	$tcase(construction_destruction, ptr_test)
	{
		int test = 0;
	
		{
			ptr<TestObject> ob = make<TestObject>(&test);

			assert_equals(test, 1);
		}

		assert_equals(test, 2);
	}

	$tcase(conversion, ptr_test)
	{
		int test = 0;

		{
			ptr<TestSubObject> ob = make<TestSubObject>(&test);

			{
				auto casted = ob.convert<TestObject>();
			}

			assert_equals(test, 1);
		}
	}
}