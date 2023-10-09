#pragma once

#include <type_traits>

using id_t = unsigned short;

template <typename ... Ts>
struct typelist
{};

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
}

template <typename T, typename TL>
constexpr id_t index_of()
{
	return detail::idx_of<0, T, TL>::value;
}