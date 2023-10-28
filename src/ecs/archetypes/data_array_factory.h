#pragma once

#include "ecs/archetypes/data_array.h"
#include "ecs/components/component.h"
#include "ecs/components/component_set.h"

#include "lib/ptr.h"
#include "lib/vector.h"

namespace ag
{
	using data_array_factory_func = ptr<data_array>(*)();

	vector<ptr<data_array>> create_arrays_for_components(const component_set<MAX_COMPONENTS>& cTypes)
	{
		vector<ptr<data_array>> out{};
		for (const id_t ct : cTypes)
		{
			out.emplace_back(_dataArrayFactoryFuncs[ct]());
		}

		return out;
	}

	constinit arr<data_array_factory_func, _componentCount> _dataArrayFactoryFuncs = _dataArrayFactoryInit<ComponentTypes>();

	template <template <typename ... Ts> typename TL>
	constexpr arr<data_array_factory_func, _componentCount> _dataArrayFactoryInit() noexcept
	{
		arr<data_array_factory_func, _componentCount> ar{};
		(ar[componentID<Ts>] = []()
			{
				return make<data_array<Ts>, data_array>();
			}
			, ...);
		return ar;
	}
}