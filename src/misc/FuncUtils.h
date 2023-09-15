#pragma once
#include <type_traits>

namespace {
	template <typename Ret, typename Arg>
	Arg unary_func_arg_helper(Ret(*)(Arg));

	template <typename F>
	decltype(unary_func_arg_helper(&F::operator())) unary_func_arg_helper(F);
}

template <typename T>
using unary_func_argument = decltype(unary_func_arg_helper(std::declval<T>()));