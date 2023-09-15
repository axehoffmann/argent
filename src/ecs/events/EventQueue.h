#pragma once

#include <vector>
#include <mutex>

#include "Event.h"

namespace ag::event
{
	class IEventQueue 
	{
	public:
		size_t size() const
		{
			return count;
		}
	private:
		size_t count;
	};
	/**
	* When an event is sent, it is added to its respective event queue. 
	* Each event listener gets to read the queue at the end of a frame. (read-only)
	*/
	template <EventType T>
	class EventQueue : IEventQueue
	{
	public:
		void push(const T& event)
		{
			std::lock_guard<std::mutex> lock(events_mutex);

			events.push_back(event);
			count++;
		}

		void clear()
		{
			std::lock_guard<std::mutex> lock(events_mutex);

			events.clear();
			count = 0;
		}

		template <typename Functor>
		void iterate(Functor func)
		{
			for (const T& event : events)
			{
				func(event);
			}
		}

	private:
		std::vector<T> events;
		std::mutex events_mutex;
	};
}