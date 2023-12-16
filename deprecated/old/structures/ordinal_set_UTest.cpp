#include "core/structures/ordinal_set.h"
#include "../../TestFramework.h"

namespace ordinal_set_test
{
	$UTest(ordinal_sets);

	$Case(subset, ordinal_sets)
	{
		// Dynamically allocated set
		agt::ordinal_set<int> dyn(4);

		dyn.insert(5);
		dyn.insert(2);
		dyn.insert(6);
		dyn.insert(8);
		dyn.insert(1); // Should be ignored
		dyn.sort();

		agt::ordinal_setn<int, 3> stc{};

		stc.insert(5);
		stc.insert(2);
		stc.sort();

		ag_expect(stc.subset_of(dyn), "Expected stc to be a subset of dyn");
		ag_expect(!dyn.subset_of(stc), "Expected dyn to not be a subset of stc");

		stc.insert(1);
		stc.sort();

		ag_expect(!stc.subset_of(dyn), "Expected stc to no longer be a subset of dyn");
	}
}