#include <type_traits>

namespace agt
{
	template<typename Arg, int size>
	struct functor 
	{
	public:

	private:
		typename std::aligned_storage<size>::type stack;
		void(*functionPtr)(const Arg&);
	};
}