#pragma once

#include "ecs/world/system.h"

#include "lib/sync.h"

#include <thread>

namespace ag
{
	class worker_thread
	{
	public:
		worker_thread(atomic<bool>& running_flag) :
			running(running_flag)
		{
			thread = std::jthread([&]()
			{
				while (running)
				{
					// Handle IO requests and any heavy tasks
				}
			});
		}

	private:
		atomic<bool>& running;
		std::jthread thread;
	};
}