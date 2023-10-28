#pragma once

#include "component.h"

#include "lib/basic.h"
#include "lib/vector.h"

#include <limits>

namespace ag
{
	template <u8 N>
	class component_set
	{
	public:
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

		constexpr bool contains(id_t id) const
		{
			for (const id_t i : ids)
			{
				if (i == id)
				{
					return true;
				}
			}

			return false;
		}

		constexpr u8 find(id_t id) const
		{
			for (const id_t i : ids)
			{
				if (i == id)
				{
					return true;
				}
			}

			return std::numeric_limits<u8>::max();
		}

		constexpr id_t* begin() const
		{
			return ids;
		}

		constexpr id_t* end() const
		{
			return ids + N;
		}

	private:
		id_t ids[N];
	};

	template <int N, typename ... Ctype>
	constexpr component_set<N> make_component_set() noexcept
	{
		int idx = 0;
		vector<id_t> ids{};
		(ids.push_back(componentID<Ctype>), ...);
		return component_set<N>(ids);
	}
}
