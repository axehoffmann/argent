#pragma once

#include <type_traits>
#include <functional>

template <typename ... Args>
using function = std::function<void(Args...)>;

template <typename ... Args>
using reader_func = std::function<void(const Args&...)>;
 
namespace agt
{
	template<int size, typename ... Args>
	struct functor 
	{
	public:

	private:
		typename std::aligned_storage<size>::type stack;
		void(*functionPtr)(const Args&...);
	};
}