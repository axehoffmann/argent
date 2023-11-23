#pragma once

#include "ecs/archetypes/data_array.h"
#include "ecs/components/component.h"
#include "ecs/components/component_set.h"

#include "lib/ptr.h"
#include "lib/vector.h"

namespace ag
{
	using data_array_factory_func = ptr<data_array>(*)();

	template <typename ... Ts>
	constexpr arr<data_array_factory_func, componentCount> _dataArrayFactoryInit(typelist<Ts...>) noexcept
	{
		arr<data_array_factory_func, componentCount> ar{};
		((ar[componentID<Ts>] = []()
			{
				return make<data_array_t<Ts>, data_array>();
			})
		, ...);
		return ar;
	}

	constinit arr<data_array_factory_func, componentCount> _dataArrayFactoryFuncs = _dataArrayFactoryInit(ComponentTypes());

	vector<ptr<data_array>> create_arrays_for_components(const component_set<MAX_COMPONENTS>& cTypes)
	{
		vector<ptr<data_array>> out{};
		for (const id_t ct : cTypes)
		{
			std::cout << ct << std::endl;

			out.emplace_back(_dataArrayFactoryFuncs[ct]());
		}

		return out;
	}
}