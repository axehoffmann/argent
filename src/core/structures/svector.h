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
			if (count == capacity)
				return;

			*(begin_p + count) = value;
			count++;
		}

		T& operator[](int index) const { return at(index); }

		T& at(int index) const
		{
			if (index < 0 || index >= count)
				throw std::out_of_range("svectorn index out of range");

			return *(begin_p + index);
		}

		class iterator
		{
			friend class svectorn;
		public:
			T& operator*() const { return *(data + index); }
			bool operator!=(iterator other) const { return data != other.data || index != other.index; }
			iterator& operator++() { index++; return *this; }
		private:
			iterator(int i, T* buff) : index(i), data(buff) {}

			int index;
			T* data;
		};

		iterator begin() { return iterator(0, begin_p); }
		iterator end() { return iterator(count, begin_p); }


	protected:
		svectorn(int n, T* begin, T* end) : begin_p(begin), end_p(end), capacity(n), count(0) {}
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
		svector() : svectorn<T>(N, (T*)buffer, (T*)buffer) {}

	private:
		char buffer[sizeof(T) * N];
	};
}