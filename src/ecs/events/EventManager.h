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
		template <EventType T>
		void pushEvent(const T& event)
		{
			EventQueue<T>* t_queue = getQueue<T>();

			t_queue->push(event);
		}

		template <typename Functor>
		void readEvents(Functor func)
		{
			using Event_T = unary_func_argument<Functor>;
			EventQueue<T>* t_queue = getQueue<T>();

			t_queue->iterate(func);
		}

	private:
		template <EventType T>
		int eventTypeID()
		{
			static int id = next_event_id.fetch_add(1);
			return id;
		}

		template <EventType T>
		EventQueue<T>* getQueue()
		{
			// Generate a queue for each registered event type
			int id = eventTypeID<T>();

			if (event_queues.size() <= id)
				event_queues.resize(event_queues.size() + 1);

			if (!event_queues.at(id))
				event_queues.at(id) = std::make_unique<EventQueue<T>>();

			EventQueue<T>* t_queue = static_cast<EventQueue<T>*>(event_queues.at(id));
		}
		std::vector<std::unique_ptr<IEventQueue>> event_queues;
		std::mutex event_queue_mutex;
		inline static std::atomic<int> next_event_id{0};
	};
}