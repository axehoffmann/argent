#pragma once

#include "ecs/world/system.h"

#include "lib/sync.h"
#include "lib/job_graph.h"

#include <thread>

namespace ag
{
	class ecs_thread
	{
	public:
		ecs_thread(atomic<bool>& running_flag, job_graph<isystem>& tasks) : 
			running(running_flag),
			jobs(tasks)
		{
			thread = std::jthread([&]()
			{
				while (running)
				{
					// System update
					jobs.consume([](temp_ptr<isystem> s)
					{
						s->update();
					});

					// Events update

					// Scene graph, audio info, network send, spatial index

					// Resolve archetype buffers (in parralel)

					// Spin until next update
				}
			});
		}

	private:
		atomic<bool>& running;
		job_graph<isystem>& jobs;
		std::jthread thread;
	};
}