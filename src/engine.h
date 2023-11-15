#pragma once

#include <barrier>
#include <thread>

#include <chrono>

#include "lib/sync.h"
#include "lib/vector.h"
#include "lib/job_graph.h"

#include "ecs/world/system.h"
#include "ecs/world/system_scheduler.h"

namespace stc = std::chrono;

namespace ag
{
	class engine
	{
		stc::microseconds tick_rate = stc::milliseconds(1000 / 60);
	public:
		engine()
			: ecs_jobs(ag::build_system_schedule())
		{
			std::thread::hardware_concurrency();
			
		}

		void begin()
		{
			atomic<bool> running = true;

			auto ecs_unit = [&]()
			{
				while (running)
				{
					// System update
					ecs_jobs->consume([](temp_ptr<isystem> s)
					{
						s->update();
					});
					
					// Events update

					// Scene graph, audio info, network send, spatial index

					// Spin until next update
				}
			};

			auto worker_unit = [&]()
			{
				// Handle IO requests and any heavy tasks
			};

			while (running)
			{
				// Render
			}
		}
	private:
		
		ptr<job_graph<isystem>> ecs_jobs;
		/// TODO: event system jobs, network update jobs

		std::thread audio_thread;
		vector<std::thread> worker_threads;
		vector<std::thread> ecs_threads;
	};
}