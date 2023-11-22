#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include <iostream>

#include "EventQueue.h"
#include "misc/FuncUtils.h"

#include "lib/functor.h"
#include "lib/tuple.h"
#include "lib/vector.h"

namespace ag::event
{
	template <typename ... Ts>
	class event_manager_generic;
}

using event_manager = ag::event::event_manager_generic<
	
>;

namespace ag::event
{
	template <typename ... Ts>
	class event_manager_generic
	{
	public:
		template <typename T>
		void pushEvent(T event)
		{
			std::get<event_queue<T>>(event_queues)->push(event);
		}

		void alertAll()
		{
			std::apply([](auto&... q){(q->alert(), ...);});
		}

		void clearAll()
		{
			std::apply([](auto&... q) {(q->clear(), ...); });
		}

		template <typename Func>
		void registerListener(Func callback)
		{
			using T = unary_func_argument<Func>;
			
			reader_func<T> func_wrap(callback);
			std::get<event_queue<T>>(event_queues)->register_listener(func_wrap);
		}

	private:
		tuple<event_queue<Ts>...> event_queues;
		std::mutex event_queue_mutex;
		inline static std::atomic<int> next_event_id{0};
	};
}