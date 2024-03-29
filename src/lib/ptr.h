#pragma once

#include <memory>
#include <type_traits>

template <typename T>
struct ptr;

/**
 * A non-owning temporary pointer
 */
template <typename T>
struct temp_ptr
{
	
	template <typename C>
	friend struct ptr;

	T* operator->() const
	{
		return object;
	}

private:
	
	temp_ptr(T* p) : object(p) {}

	T* object;
};

/**
 * An owning, non-unique, non-null pointer
 */
template <typename T>
struct ptr
{
	template <typename T, typename ... Params>
	friend ptr<T> make(Params... params);

	template <typename T, typename Base, typename ... Params>
	friend ptr<Base> make(Params... params);

	T* operator->() const
	{
		return object;
	}

	
	template <typename new_T>
	temp_ptr<new_T> convert() const
	{
		static_assert(std::is_convertible_v<T, new_T>);

		return temp_ptr<new_T>(static_cast<new_T*>(object));
	}

	temp_ptr<T> get_temp() const
	{
		return temp_ptr<T>{.object = object};
	}
	

	ptr(const ptr&) = delete;
	ptr& operator=(const ptr&) = delete;

	ptr(ptr&& other)
	{
		object = other.object;
		other.object = nullptr;
	}


	~ptr()
	{
		delete object;
	}

private:
	template <typename ... Params>
	ptr(Params... params) : object(new T(params...)) {}

	ptr(T* ob) : object(ob) {}

	T* object;
};

template <typename T, typename ... Params>
ptr<T> make(Params... params)
{
	return ptr<T>(params...);
}

template <typename T, typename Base, typename ... Params>
ptr<Base> make(Params... params)
{
	return ptr<Base>(static_cast<Base*>(new T(params...)));
}