#include "system_scheduler.h"

#include "lib/set.h"

#include <algorithm>

using namespace ag;

using bucket = arr<vector<u16>, componentCount>;
using node = job_graph<isystem>::node;


auto build_system_schedule() -> job_graph<isystem>
{
	auto& systems = isystem::registers();

	/// TODO: can optimise to give readers concurrency.

	u16 sentinel = systems.size();

	// bucket writers;
	// bucket readers;
	
	vector<u16> to_process(systems.size());

	vector<u16> entry_nodes;

	vector<node> nodes(systems.size());
	for (u16 i = 0; const pair<query_set, isystem::factory_func>& x : systems)
	{
		nodes.emplace_back(std::move(x.second()), vector(), 0, i);
		to_process(i);

		/*
		// Populate buckets to help us build schedule later
		const query_set& qs = x.first;
		for (u8 i = 0; i < qs.size(); i++)
		{
			bucket& buck = qs.get(i).is_readonly
				? readers
				: writers;
			buck.at(qs.get(i).id).push_back(i);
		}
		*/
		i++;
	}

	// Sort with the least-constraining queries first ie. AB, ACD, A -> A, AB, ACD
	std::sort(std::begin(to_process), std::end(to_process), [](u16 a, u16 b) -> bool
		{
			return systems[a].first.size() < systems[b].first.size();
		});

	// The nodes in the graph with the most recent write occurrence of each component type (ie, any further nodes will need to depend on these)
	vector<u16> terminals(componentCount, sentinel);

	for (u16 sys : to_process)
	{
		for (id_t ctype : systems[sys].first.read())
		{
			// We depend on a system already placed in the graph
			if (terminals[ctype] != sentinel)
			{
				nodes[terminals[ctype]].children.push_back(sys);
				nodes[sys].dependencies += 1;
			}

			terminals[ctype] = sys;
		}
	}

	return job_graph<isystem>(std::move(nodes));
}
