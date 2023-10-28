#pragma once

#include "ecs/components/component_set.h"
#include "ecs/world/query.h"
#include "lib/vector.h"
#include "lib/ptr.h"
#include "lib/tuple.h"

namespace ag
{
	class isystem
	{
		using factory_func = ptr<isystem>(*)();

		/**
		 * Static registry for systems
		*/
		static auto registers()
		{
			static vector<pair<component_set<max_query>, factory_func>> reg;
			return reg;
		}

		/// TODO: could do compile time registering
		template <typename T>
		struct sys_register
		{
			sys_register()
			{
				registers().push_back(pair(
					T::query_types,
					[]() -> ptr<isystem>
					{
						return make<T, isystem>();
					}
				))
			}
		};
	};
}

template <class T, template <typename ... Cs> typename Q>
class system : public ag::isystem {};

template <class Impl, typename ... Cs>
class system<Impl, query<Cs...>> : public ag::isystem
{
public:
	constexpr ag::component_set<max_query> query_types = ag::make_component_set<Cs...>();
private:
	static inline auto reg = ag::isystem::sys_regiser<Impl>();
};