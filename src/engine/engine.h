#pragma once

#include <barrier>
#include <thread>

#include <chrono>

#include "lib/sync.h"
#include "lib/vector.h"
#include "lib/job_graph.h"

#include "ecs/world/system.h"
#include "ecs/world/system_scheduler.h"

#include "engine/ecs_thread.h"
#include "engine/worker_thread.h"

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
			// calculate amount of game/worker threads
			std::thread::hardware_concurrency();
			
		}

		void begin()
		{
			atomic<bool> running = true;

			vector<ecs_thread> ecs_threads(ecs_thread_count, ecs_thread(running, ecs_jobs));
			vector<worker_thread> worker_threads(worker_thread_count, worker_thread(running));

			while (running)
			{
				// Render
			}
		}
	private:
		
		job_graph<isystem> ecs_jobs;
		/// TODO: event system jobs, network update jobs

		std::thread audio_thread;
		vector<std::thread> worker_threads;

		u8 ecs_thread_count;
		u8 worker_thread_count;

	};
}