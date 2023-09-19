#pragma once
#include <type_traits>

namespace 
{
	template <typename F, typename Ret, typename Arg>
	Arg unary_func_arg_helper(Ret(F::*)(const Arg&) const);

	template <typename F, typename Ret, typename Arg>
	Arg unary_func_arg_helper(Ret(F::*)(const Arg&));
}

template <typename T>
using unary_func_argument = decltype(unary_func_arg_helper(&T::operator()));