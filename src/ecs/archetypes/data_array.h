#pragma once

#include "ecs/components/component.h"
#include "lib/basic.h"
#include "lib/dyn_array.h"
#include "lib/vector.h"

#include <bit>
#include <cassert>
#include <cstring>

namespace ag
{
	/**
	 * Type-erased array of components
	*/
	class data_array
	{
	public:
		/**
		 * Gets a reference to a component in the array
		 * @tparam T	the type of the component. Must be equal to the true underlying type of the array
		 * @param idx	the index into the array. Must be within the valid range of the array
		 * @return		a reference to the component
		*/
		template <typename T>
		T& get(u64 idx) const
		{
			assert(idx <= size);
			assert(componentID<T> == componentType);

			return *(static_cast<T*>(start) + idx);
		}

		/**
		 * Gets a const reference to a component in the array
		 * @tparam T	the type of the component. Must be equal to the true underlying type of the array
		 * @param idx	the index into the array. Must be within the valid range of the array
		 * @return		a const reference to the component
		*/
		template <typename T>
		const T& get(u64 idx) const
		{
			assert(idx <= size);
			assert(componentID<T> == componentType);

			return *(reinterpret_cast<T*>(start) + idx);
		}

		/**
		 * Gets a pointer to the first object in the array
		 * @tparam T	the type to fetch. Must be equal to the underlying type of the array
		 * @return		a pointer to the first component in the array
		*/
		template <typename T>
		T* begin() const
		{
			assert(componentID<T> == componentType);

			return reinterpret_cast<T*>(start);
		}

		/**
		 * Erases elements at a set of indices from the array.
		 * @param indices a range of indices to erase.
		*/
		void eraseIndices(const dyn_array<u64>& indices)
		{
			for (u64 idx : indices)
			{
				std::cout << idx << " ";
			}
			std::cout << "\n";

			auto rm_it = indices.begin();
			u64 currentIdx = 0;

			const auto predicate = [&]()
			{
				if (rm_it == indices.end()) return false;
				if (*rm_it == currentIdx++) return ++rm_it, true;
				return false;
			};

			// Find first element to remove (variable-size stepping std::find-if)
			auto first = [&]()
			{
				auto pos = start;
				while (pos < end)
				{
					if (predicate()) return pos;
					pos += componentSize;
				}
			}();

			// std::remove_if with variable-size steps
			if (first != end)
			{
				for (auto it = first; (it += componentSize) != end;)
				{
					if (!predicate())
					{
						std::memcpy(first, it, componentSize);
						first += componentSize;
					}
				}
			}

			end = first;
		}

		/**
		 * Inserts a series of type-erased values into the array.
		 * - The data must have the same underlying type as this array. This is not validated at runtime for performance.
		 * - The data may exceed the current capacity of the array
		 * @param data a view into the data to insert
		*/
		void insert(byte* data, u32 bytes)
		{
			u64 newSz = count + bytes / componentSize;

			// Do we need to reallocate to fit this?
			if (newSz > size)
			{
				size = std::bit_ceil(newSz);
				reallocate();
			}
			count += bytes / componentSize;
			std::memcpy(end, data, bytes);
			end += bytes;
		}
		
		virtual ~data_array() {}

		data_array(id_t type) : componentType(type), componentSize(component_size(type)) {}

	protected:
		virtual void reallocate() = 0;

		u64 count{ 0 };
		u64 size{ 0 };

		byte* start{ nullptr };
		byte* end{ nullptr };

	private:
		id_t componentType;
		u16 componentSize;

	};

	template <typename T>
	class data_array_t : public data_array
	{
	public:
		data_array_t() : data_array(componentID<T>), first(nullptr) {}

	protected:
		void reallocate() override
		{
			T* newBuf = new T[size];

			if (first != nullptr)
			{
				std::memcpy(newBuf, first, count * sizeof(T));
				delete[] first;
			}

			first = newBuf;
			start = reinterpret_cast<byte*>(first);
			end = reinterpret_cast<byte*>(first + count);
		}

	private:
		T* first;
	};
}