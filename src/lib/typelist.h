#pragma once

#include "lib/basic.h"

#include <type_traits>

/**
 * A typelist and set of constexpr helpers used to calculate component IDs and sizes at compile time.
*/

using id_t = unsigned short;

template <typename ... Ts>
struct typelist
{
	constexpr typelist() {}
};

namespace detail
{
	template <id_t N, typename T, typename ... Ts>
	struct idx_of;

	template <id_t N, typename T, typename C>
	struct idx_of<N, T, C>
	{
		constexpr static inline id_t value = (std::is_same_v<T, C>) ? N : -1;
	};

	template <id_t N, typename T, typename C, typename ... Ts>
	struct idx_of<N, T, typelist<C, Ts...>>
	{
		constexpr static inline id_t value = (std::is_same_v<T, C>) ? N : idx_of<N + 1, T, typelist<Ts...>>::value;
	};

	/*
	template <typename T, typename T2, typename ... Tn>
	constexpr u16 size_of(id_t id, id_t cur) noexcept
	{
		if (id == cur)
		{
			return sizeof(T);
		}

		return detail::size_of<T2, Tn...>(id, cur + 1);
	}

	template <typename T>
	constexpr u16 size_of(id_t id, id_t cur) noexcept
	{
		if (id == cur)
		{
			return sizeof(T);
		}

		return 0;
	}
	*/
}

template <typename T, typename TL>
constexpr id_t index_of() noexcept
{
	return detail::idx_of<0, T, TL>::value;
}

/*
template <template <typename ... Ts> typename TL>
constexpr u16 size_of(id_t id) noexcept
{
	return detail::size_of<Ts...>(id, 0);
}
*/

template <template <typename ... Ts> typename TL>
constexpr id_t type_count() noexcept
{
	return sizeof...(Ts);
}