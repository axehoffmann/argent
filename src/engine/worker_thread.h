#pragma once

#include "ecs/world/system.h"

#include "lib/sync.h"

#include "resources/v2/resrc_manager.h"

#include <thread>

namespace ag
{
	class worker_thread
	{
	public:
		worker_thread(atomic<bool>& running_flag, resrc_manager& man) :
			running(running_flag),
			manager(man)
		{
			thread = std::jthread([&]()
			{
				while (running)
				{
					// Handle IO requests and any heavy tasks
					manager.do_work();
				}
			});
		}

	private:
		atomic<bool>& running;
		resrc_manager& manager;

		std::jthread thread;
	};
}