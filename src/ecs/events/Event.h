#pragma once

#include <type_traits>

namespace ag::event
{
	struct IEvent
	{

	};

	template <typename T>
	concept EventType = std::is_base_of_v<IEvent, T>;

	template <typename T>
	struct Event
	{

	};
}