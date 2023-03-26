#pragma once

#include "ECSTypes.h"
#include <atomic>
#include <unordered_map>

namespace ag
{
	class Component
	{
	public:
		// Generates a unique ID for each type
		// trick from @nice_byte
		template <typename T>
		static ComponentTypeID GetID()
		{
			static ComponentTypeID id = ++nextComponentID;
			componentSize[id] = sizeof(T);
			return id;
		}

		static int GetSize(ComponentTypeID id);

	private:
		static std::atomic<ComponentTypeID> nextComponentID;
		static std::unordered_map<ComponentTypeID, int> componentSize;
		
	};
}

