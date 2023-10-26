#pragma once

#include "ecs/components/component_set.h"
#include "ecs/archetypes/data_array.h"
#include "ecs/archetypes/data_array_factory.h"


#include "lib/ptr.h"
#include "lib/vector.h"

namespace ag
{
	class archetype
	{
	public:

		archetype(const component_set<MAX_COMPONENTS>& cTypes) noexcept 
			: componentTypes(cTypes),
			  dataArrays(create_arrays_for_components(cTypes))
		{
			
		}


		/**
		 * An iterator that can iterate across multiple archetypes,
		 * as long as each archetype has a subset of components Ts...
		 * @tparam ...Ts the subset of components that this iterator accesses
		*/
		template <typename ... Ts>
		struct iterator 
		{

		};

	private:
		component_set<MAX_COMPONENTS> componentTypes;
		vector<ptr<data_array>> dataArrays;
	};
}