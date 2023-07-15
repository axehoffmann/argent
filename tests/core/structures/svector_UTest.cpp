#include "core/structures/svector.h"
#include "../../TestFramework.h"

namespace svector_test
{
	$UTest(svectors);

	$Case(data_access, svectors)
	{
		agt::svector<int, 4> vec;

		for (int i = 0; i < 4; i++)
		{
			vec.push_back((2 * i) + 4);
		}

		// indexed iteration
		for (int i = 0; i < 4; i++)
		{
			ag_expect(vec.at(i) == ((2 * i) + 4), "Expected value at {} to be {}, instead found {}", i, ((2 * i) + 4), vec.at(i));
		}

		// foreach iteration
		int x = 0;
		for (int& i : vec)
		{
			ag_expect(i == (2 * x) + 4, "Expected value at {} to be {}, instead found {}", x, ((2 * x) + 4), i);
			x++;
		}
	}

	int sum(agt::svectorn<int> vec)
	{
		int result = 0;
		for (int i : vec)
		{
			result += i;
		}
		return result;
	}

	$Case(type_erasure, svectors)
	{
		// Passing a svector as a parameter and erasing its type
		// Half-full svector
		agt::svector<int, 8> vec1;
		for (int i = 0; i < 4; i++)
		{
			vec1.push_back(2 * i);
		}
		
		ag_expect(sum(vec1) == 12, "Expected sum of half-full svector to be 12, instead found {}", sum(vec1));

		// Full svector
		agt::svector<int, 6> vec2;
		for (int i = 0; i < 6; i++) 
		{
			vec2.push_back(5);
		}

		ag_expect(sum(vec2) == 30, "Expected sum of full svector to be 30, instead found {}", sum(vec2));
	}
}