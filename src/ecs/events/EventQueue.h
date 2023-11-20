#pragma once

#include <vector>
#include <mutex>
#include <functional>

#include "Event.h"

#include "lib/vector.h"
#include "lib/functor.h"

namespace ag::event
{
	/**
	* When an event is sent, it is added to its respective event queue. 
	* Each event listener gets to read the queue at the end of a frame. (read-only)
	*/
	template <typename T>
	class event_queue
	{
	public:
		size_t size() const
		{
			return events.size();
		}

		void push(T&& e)
		{
			std::lock_guard<std::mutex> lock(events_mutex);
			events.push_back(e);
		}

		void clearEvents()
		{
			std::lock_guard<std::mutex> lock(events_mutex);
			events.clear();
		}

		void readEvents()
		{
			for (auto& listener : listeners)
			{
				iterate(listener);
			}
		}

		void registerListener(const std::function<void(const T&)>& func)
		{
			listeners.push_back(func);
		}

	private:
		vector<T> events;
		std::mutex events_mutex;
		
		// TODO: may have to synchronise this
		vector<reader_func<T>> listeners;

		void iterate(auto& func) const
		{
			for (const T& event : events)
			{
				func(event);
			}
		}
	};
}