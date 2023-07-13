#pragma once

#include <stdexcept>

namespace agt
{
	// Inspired by llvm/Chandler Carruth's hybrid data structures talk

	template <typename T>
	class svectorn
	{
	public:
		void push_back(const T& value)
		{
			*(begin_p + count) = value;
			count++;
		}

		T& at(int index)
		{
			if (index < 0 || index >= count)
				throw std::out_of_range("svectorn index out of range");

			return *(begin_p + count - 1);
		}

	protected:
		svectorn(int n, T* begin, T* end) : begin_p(buff), end_p(buff), capacity(n), count(0) {}
	private:
		T* begin_p;
		T* end_p;
		int capacity;
		int count;
	};

	template <typename T, int N>
	class svector : public svectorn<T>
	{
	public:
		svector() : svectorn(N, (T*)buffer, (T*)buffer) {}

	private:
		char buffer[sizeof(T) * N];
	};
}