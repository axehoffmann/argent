#include <type_traits>
#include <functional>

template <typename ... Args>
using functor = std::function<void(Args&...)>;

namespace agt
{
	template<typename ... Args, int size>
	struct functor 
	{
	public:

	private:
		typename std::aligned_storage<size>::type stack;
		void(*functionPtr)(const Args&...);
	};
}