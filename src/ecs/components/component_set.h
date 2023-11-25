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
			size = other.size;
			std::copy(other.ids, other.ids + N, ids);
		}

		constexpr component_set(vector<id_t>&& data)
		{
			size = data.size();
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
			for (const id_t x : ids)
			{
				if (x == id)
				{
					return true;
				}
			}

			return false;
		}

		constexpr u8 find(id_t id) const
		{
			for (u8 i = 0; const id_t x : ids)
			{
				if (x == id)
				{
					return i;
				}
				i++;
			}

			return std::numeric_limits<u8>::max();
		}
		
		constexpr id_t at(u8 idx) const
		{
			return ids[idx];
		}

		constexpr const id_t* begin() const
		{
			return ids;
		}

		constexpr const id_t* end() const
		{
			return ids + size;
		}

	private:
		u8 size;
		id_t ids[N];
	};

	template <u8 N, typename ... Ctype>
	constexpr component_set<N> make_component_set() noexcept
	{
		u8 idx = 0;
		vector<id_t> ids{};

		(ids.emplace_back(id_t(componentID<std::remove_cv_t<Ctype>>)), ...);

		return component_set<N>(std::move(ids));
	}
}
