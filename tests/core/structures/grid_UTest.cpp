#include "../../TestFramework.h"
#include "core/structures/grid.h"

namespace grid_test
{
	$UTest(grid);

	$Case(box_query, grid)
	{
		agt::grid<int> gr(5, 5, 1);

		size_t count;
		count = gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size();
		ag_expect(count == 0, "Expected 0 results when querying an empty grid, found {}", count);

		gr.insert(1, { 1.0f, 1.0f });
		count = gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size();
		ag_expect(count == 1, "Expected 1 result when querying an occupied grid, found {}", count);
		// Outside our query zone
		gr.insert(1, { 1.6f, 3.5f });
		count = gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size();
		ag_expect(count == 1, "Expected 1 result when querying an occupied grid, found {}", count);

		gr.insert(1, { 0.5f, 0.5f });
		gr.insert(1, { 1.5f, 1.5f });
		count = gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }).size();
		ag_expect(count == 3, "Expected 3 results when querying an occupied grid, found {}", count);

		// Move query over
		count = gr.query_box({ 1.5f, 1.5f }, { 4.0f, 4.0f }).size();
		ag_expect(count == 2, "Expected 2 results when querying an occupied grid, found {}", count);

		count = gr.query_box({ 1.6f, 1.6f }, { 4.0f, 4.0f }).size();
		ag_expect(count == 1, "Expected 1 result when querying an occupied grid, found {}", count);
	}

	$Case(box_query_with_predicate, grid)
	{
		agt::grid<int> gr(5, 5, 1);

		size_t count;

		gr.insert(1, { 0.5f, 0.5f });
		gr.insert(5, { 1.0f, 1.0f });
		gr.insert(3, { 1.5f, 1.0f });
		gr.insert(2, { 1.2f, 1.0f });
		gr.insert(8, { 1.3f, 1.1f });
		// Check with predicate for even numbers
		count = gr.query_box({ 0.0f, 0.0f }, { 2.0f, 2.0f }, 
			[](auto x) { return x.val % 2 == 0; })
			.size();
		ag_expect(count == 2, "Expected 2 results when querying with predicate, found {}", count);
	}
}