#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include <iostream>

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

		void alertAll()
		{
			for (auto& queue : event_queues)
			{
				queue->alert();
			}
		}

		void clearAll()
		{
			for (auto& queue : event_queues)
			{
				queue->clear();
			}
		}

		template <typename Func>
		void registerListener(Func callback)
		{
			using T = unary_func_argument<Func>;
			EventQueue<T>* t_queue = getQueue<T>();

			std::function<void(const T&)> func_wrap(callback);
			t_queue->register_listener(func_wrap);
		}

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

			if (id >= event_queues.size() - 1)
			{
				auto new_ptr = std::unique_ptr<IEventQueue>(new EventQueue<T>);
				std::lock_guard<std::mutex> lock(event_queue_mutex);

				event_queues.reserve(id + 1);
				event_queues.at(id) = std::move(new_ptr);
			}
			// TODO: may cause issues if generating same event T multiple times concurrently?
			else if (!event_queues.at(id))
			{
				auto new_ptr = std::unique_ptr<IEventQueue>(new EventQueue<T>);

				std::lock_guard<std::mutex> lock(event_queue_mutex);
				event_queues.at(id) = std::move(new_ptr);
			}

			EventQueue<T>* t_queue = static_cast<EventQueue<T>*>(event_queues.at(id).get());

			return t_queue;
		}

		std::vector<std::unique_ptr<IEventQueue>> event_queues;
		std::mutex event_queue_mutex;
		inline static std::atomic<int> next_event_id{0};
	};
}