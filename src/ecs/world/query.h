#pragma once

#include "lib/basic.h"
#include "lib/bitmap.h"
#include "lib/tuple.h"
#include "ecs/components/component_set.h"

constexpr u8 max_query = 8;

template <typename ... Cs>
class query {};

namespace ag
{
	struct ctype
	{
		id_t id;
		bool is_readonly;
	};

	class query_set
	{
		template <typename ... Cs>
		friend query_set make_query_set(Cs...);

	public:
		ctype get(u8 idx) const
		{
			return ctype{ types.at(idx), is_readonly.test(idx) };
		}

		u8 size() const
		{
			return len;
		}

		const component_set<max_query>& read() const
		{
			return types;
		}

	private:
		u8 len;
		component_set<max_query> types;
		bitmap<max_query> is_readonly;
	};

	template <typename ... Cs>
	constexpr query_set make_query_set()
	{
		query_set qs;

		qs.types = make_component_set<max_query, Cs...>();
		u8 idx = 0;
		((qs.is_readonly.set(idx, std::is_const_v<Cs>), idx++), ...);
		qs.len = idx;

		return qs;
	}
}