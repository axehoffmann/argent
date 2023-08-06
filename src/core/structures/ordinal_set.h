#pragma once

#include <algorithm>

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

			/// TODO: reevaluate when not about to fall asleep
			/// We should be able to make an early exit in some cases
			while (i < size() && j < other.size())
			{
				if (at(i) == other.at(index_in_other))
				{
					i++;
					j++;
					continue;
				}

				j++;
			}
			
			return i + 1 < size();
		}

		void sort()
		{
			std::sort(begin_p, end_p);
		}

		void insert(const T& value)
		{
			if (count == capacity)
				return;

			*(begin_p + count) = value;
			count++;
		}

	protected:
		ordinal_set(int n, T* begin, T* end) : begin_p(begin), end_p(end), capacity(n), count(0) {}
	private:
		T* begin_p;
		T* end_p;
		int capacity;
		int count;

		T& at(int index) const
		{
			if (index < 0 || index >= count)
				throw std::out_of_range("index out of range");

			return *(begin_p + index);
		}
	}

	template <typename T, int N>
	class ordinal_set : public ordinal_set<T>
	{
	public:
		ordinal_set() : ordinal_set<T>(N, (T*)buffer, (T*)buffer) {}

	private:
		char buffer[sizeof(T) * N];
	}
}