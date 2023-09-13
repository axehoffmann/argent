#pragma once

#include <type_traits>

namespace ag::event
{
	template <typename T>
	concept EventType = std::is_base_of_v<IEvent, T>;

	struct IEvent
	{

	};

	template <typename T>
	struct Event
	{

	};
}