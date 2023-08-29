#include "../../TestFramework.h"
#include "core/structures/grid.h"

namespace grid_test
{
	$UTest(grid);

	$Case(box_query, grid)
	{
		agt::grid<int> gr(5, 5, 1);

		ag_expect(gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size() == 0, "Expected 0 results when querying an empty grid");

		gr.insert(1, { 1.0f, 1.0f });
		ag_expect(gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size() == 1, "Expected 1 result when querying an occupied grid");

		gr.insert(1, { 0.5f, 0.5f });
		gr.insert(1, { 1.5f, 1.5f });
		ag_expect(gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size() == 1, "Expected 3 results when querying an occupied grid");

	}
}