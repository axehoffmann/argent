#include "ecs/components/component.h"
#include "lib/basic.h"
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

			return *(static_cast<T*>(data) + idx);
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

			return *(static_cast<T*>(data) + idx);
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

			return static_cast<T*>(data)
		}

		/**
		 * Inserts a series of type-erased values into the array.
		 * - The data must have the same underlying type as this array. This is not validated at runtime.
		 * - The data may exceed the current capacity of the array
		 * @param data a view into the data to insert
		*/
		void insert(range<byte>&& data)
		{
			count += data.width() / componentSize;
			// Do we need to reallocate to fit this?
			if (count > size)
			{
				size = std::bit_ceil(count);
				reallocate();
			}

			std::memcpy(end, data.begin(), data.width());
			end += data.width();
		}

	protected:
		data_array(id_t type) : componentType(type), componentSize(component_size(type)) {}
		virtual ~data_array() {}

		virtual void reallocate() = 0;

		u64 count{ 0 };
		u64 size{ 1 };
		byte* data;

	private:
		id_t componentType;
		u16 componentSize;

		byte* end;
	};

	template <typename T>
	class data_array_t : public data_array
	{
	public:
		data_array_t() : data_array(componentID<T>) {}

	protected:
		void reallocate() override
		{
			T* newBuf = new T[size];

			std::memcpy(newBuf, first, count * sizeof(T));

			delete[] first;
			first = newBuf;
			data = static_cast<byte*>(first);
		}

	private:
		T* first;
	};
}