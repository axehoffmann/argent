#include "system_scheduler.h"

#include <algorithm>

using namespace ag;

using bucket = arr<vector<u16>, componentCount>;
using node = job_graph<isystem>::node;


auto build_system_schedule() -> ptr<job_graph<isystem>>
{
	auto& systems = isystem::registers();

	bucket writers;
	bucket readers;
	
	vector<node> nodes(systems.size());
	for (u16 i = 0; const pair<query_set, isystem::factory_func>& x : systems)
	{
		// Generate unconnected graph
		nodes.emplace_back(std::move(x.second()), vector(), 0, i);

		// Populate buckets to help us build schedule later
		const query_set& qs = x.first;
		for (u8 i = 0; i < qs.size(); i++)
		{
			bucket& buck = qs.get(i).is_readonly
				? readers
				: writers;
			buck.at(qs.get(i).id).push_back(i);
		}

		i++;
	}




	return make<job_graph<isystem>>();
}
