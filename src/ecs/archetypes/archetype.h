#pragma once

#include "ecs/components/component_set.h"

using component_count_t = u8;
constexpr component_count_t MAX_COMPONENTS = 32;

namespace ag
{
	class archetype
	{
	public:

		archetype(const component_set<MAX_COMPONENTS>& cTypes) noexcept : componentTypes(cTypes)
		{
			
		}

	private:
		component_set<MAX_COMPONENTS> componentTypes;

	};
}