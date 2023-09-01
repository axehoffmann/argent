#pragma once

#include <algorithm>
#include <memory>

#include <iostream>

namespace agt
{
	template <typename T>
	class ordinal_set
	{
	public:
		int size() const
		{
			return count;
		}

		bool subset_of(ordinal_set<T>& other)
		{
			if (other.size() < size())
				return false;

			int i = 0, j = 0;

			while (i < size() && j < other.size())
			{
				if (at(i) == other.at(j))
				{
					i++;
					j++;
					continue;
				}

				j++;
			}
			
			return j <= size();
		}

		void sort()
		{
			std::sort(begin_p, begin_p + count);
		}

		void insert(const T& value)
		{
			if (count == capacity)
				return;

			*(begin_p + count) = value;
			count++;
		}

		~ordinal_set()
		{
			if (dynamic)
				delete[] begin_p;
		}

		/// TODO: does end_p need +1?
		/**
		* Create a dynamically-allocated ordinal set.
		*/
		ordinal_set(int n) : begin_p(new T[n]), end_p(begin_p + n), capacity(n), count(0), dynamic(true) {}

	protected:
		ordinal_set(int n, T* begin, T* end) : begin_p(begin), end_p(end), capacity(n), count(0), dynamic(false) {}
	private:
		T* begin_p;
		T* end_p;
		int capacity;
		int count;
		bool dynamic;

		T& at(int index) const
		{
			//if (index < 0 || index >= count)
			//	throw std::out_of_range("index out of range");

			return *(begin_p + index);
		}
	};

	template <typename T, int N>
	class ordinal_setn : public ordinal_set<T>
	{
	public:
		ordinal_setn() : ordinal_set<T>(N, (T*)buffer, (T*)buffer) {}

	private:
		char buffer[sizeof(T) * N];
	};
}