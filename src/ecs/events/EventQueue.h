#pragma once

#include <vector>
#include <mutex>
#include <functional>

#include "Event.h"

#include "lib/vector.h"
#include "lib/functor.h"

namespace ag::event
{
	class IEventQueue 
	{
	public:
		size_t size() const
		{
			return count;
		}

		void clear()
		{
			clear_ptr(this);
		}

		void alert()
		{
			alert_ptr(this);
		}

	protected:
		using ErasedFunc = void(*)(IEventQueue*);

		IEventQueue(ErasedFunc cl_ptr, ErasedFunc al_ptr) : clear_ptr(cl_ptr), alert_ptr(al_ptr) {}

		size_t count{ 0 };

		ErasedFunc clear_ptr;
		ErasedFunc alert_ptr;
	};

	/**
	* When an event is sent, it is added to its respective event queue. 
	* Each event listener gets to read the queue at the end of a frame. (read-only)
	*/
	template <typename T>
	class EventQueue : public IEventQueue
	{
	public:
		EventQueue() : IEventQueue(clear_events, read_events) {}

		void push(const T& event)
		{
			std::lock_guard<std::mutex> lock(events_mutex);

			events.push_back(event);
			count++;
		}

		static void clear_events(IEventQueue* queue)
		{
			EventQueue<T>* t_queue = static_cast<EventQueue<T>*>(queue);
			std::lock_guard<std::mutex> lock(t_queue->events_mutex);

			t_queue->events.clear();
			t_queue->count = 0;
		}

		static void read_events(IEventQueue* queue)
		{
			EventQueue<T>* t_queue = static_cast<EventQueue<T>*>(queue);

			for (auto& listener : t_queue->listeners)
			{
				t_queue->iterate(listener);
			}
		}

		void register_listener(const std::function<void(const T&)>& func)
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