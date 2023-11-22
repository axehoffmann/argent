#pragma once

#include "lib/typelist.h"
#include "lib/basic.h"
#include "lib/vector.h"

#include "debug/log/Log.h"

#include "core/Transform.h"

using ComponentTypes = typelist<
	ag::Transform
>;

using component_count_t = u8;
constexpr component_count_t MAX_COMPONENTS = 32;

namespace ag
{
	/**
	 * A compile-time variable template holding the component ID of each type.
	 * @tparam C the component type
	*/
	template <typename C>
	constexpr id_t componentID = index_of<C, ComponentTypes>();


	
	/**
	 * A type-erased non-owning component data reference
	*/
	struct component
	{
		/**
		 * Gets a pointer to the underlying data of the component.
		 * @tparam T	The expected true type of this component.
		 * @return		A pointer to the underlying data, or nullptr if the type is not expected.
		*/
		template <typename T>
		constexpr T* get() const noexcept
		{
			if (type != componentID<T>)
			{
				return nullptr;
			}

			return static_cast<T*>(data);
		}

	private:
		void* data;
		id_t type;
	};

	constexpr id_t componentCount = type_count(ComponentTypes());

	template <typename ... Ts>
	constexpr arr<u16, componentCount> _componentSizesInit(typelist<Ts...>) noexcept
	{
		arr<u16, componentCount> ar{};
		((ar[componentID<Ts>] = sizeof(Ts)), ...);
		return ar;
	}

	constinit arr<u16, componentCount> _componentSizes = _componentSizesInit(ComponentTypes());
	
	/**
	 * A compile-time calculation of the size of a component based on its ID.
	 * @param id	the ID of the component type
	 * @return		the size of the component in bytes, or 0 if the ID was invalid.
	*/
	constexpr u16 component_size(id_t id) noexcept
	{
		return _componentSizes[id];
	}
	
}