#pragma once

#include <memory>
#include <vector>

#include "EventQueue.h"
#include "misc/FuncUtils.h"

namespace ag::event
{
	class EventManager
	{
	public:
		template <typename T>
		void pushEvent(T event)
		{
			EventQueue<T>* t_queue = getQueue<T>();

			t_queue->push(event);
		}

		template <typename Functor>
		void readEvents(Functor func)
		{
			using Event_T = unary_func_argument<Functor>;
			EventQueue<Event_T>* t_queue = getQueue<Event_T>();

			t_queue->iterate(func);
		}

		template <typename T>
		void clearEvents()
		{
			EventQueue<T>* t_queue = getQueue<T>();

			t_queue->clear();
		}

		/// TODO: Event clearing - this is tough because we can't be templated + virtual = blegh?
		/// TODO: Event listener registering

	private:
		template <typename T>
		int eventTypeID()
		{
			static int id = next_event_id.fetch_add(1);
			return id;
		}

		template <typename T>
		EventQueue<T>* getQueue()
		{
			// Generate a queue for each registered event type
			int id = eventTypeID<T>();

			if (event_queues.size() <= id)
				event_queues.resize(event_queues.size() + 1);

			if (!event_queues.at(id))
				event_queues.at(id) = std::unique_ptr<IEventQueue>(new EventQueue<T>);

			EventQueue<T>* t_queue = static_cast<EventQueue<T>*>(event_queues.at(id).get());

			return t_queue;
		}

		std::vector<std::unique_ptr<IEventQueue>> event_queues;
		std::mutex event_queue_mutex;
		inline static std::atomic<int> next_event_id{0};
	};
}