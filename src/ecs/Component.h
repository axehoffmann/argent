#pragma once

#include "ECSTypes.h"
#include <atomic>
#include <unordered_map>

namespace ag
{
	/**
	 * Static class used to keep track of component type details.
	*/
	class Component
	{
	public:
		/**
		 * Allocates a unique ID for each type of component registered with this function.
		 * @tparam The type of the component
		 * @return The ID of the component type
		*/
		template <typename T>
		static ComponentTypeID GetID()
		{
			// Generates a unique ID for each type
			// trick from @nice_byte
			static ComponentTypeID id = ++nextComponentID;
			componentSize[id] = sizeof(T);
			return id;
		}

		static int GetSize(ComponentTypeID id);

	private:
		static std::atomic<ComponentTypeID> nextComponentID;
		/// TODO: Evaluate if this map is necessary.
		static std::unordered_map<ComponentTypeID, int> componentSize;
		
	};
}

