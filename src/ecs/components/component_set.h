#pragma once

#include "component.h"

#include "lib/basic.h"
#include "lib/vector.h"

namespace ag
{
	template <u8 N>
	class component_set
	{
	public:
		template <typename ... Ctype>
		constexpr component_set() noexcept
		{
			int idx = 0;
			auto pack = [&idx, &ids](id_t value)
			{
				ids[idx] = value;
				idx++;
			};
			(pack(componentID<Ctype>), ...);
		}

		constexpr component_set<N>(const component_set<N>& other)
		{
			std::copy(other.ids, other.ids + N, ids);
		}

		constexpr component_set(range<id_t> data)
		{
			std::copy(data.begin(), data.end(), ids);
		}

		constexpr bool operator==(const component_set<N>& other) const
		{
			for (u8 i = 0; i < N; i++)
			{
				if (ids[i] != other.ids[i])
				{
					return false;
				}
			}
			return true;
		}

	private:
		id_t ids[N];
	};
}
