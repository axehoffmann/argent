#pragma once

#include <vector>
#include <mutex>

#include "Event.h"

namespace ag::event
{
	/**
	* When an event is sent, it is added to its respective event queue. 
	* Each event listener gets to read the queue at the end of a frame. (read-only)
	*/
	template <EventType T>
	class EventQueue
	{
	public:
		void push(const T& event)
		{
			std::lock_guard<std::mutex> lock(events_mutex);

			events.push_back(event);
		}

		void clear()
		{
			std::lock_guard<std::mutex> lock(events_mutex);

			events.clear();
		}

	private:
		std::vector<T> events;
		std::mutex events_mutex;
	};
}