#pragma once

#include <mutex>
#include <atomic>

using mutex = std::mutex;

template <typename T>
using lock_guard = std::lock_guard<T>;

template <typename T>
using atomic = std::atomic<T>;


/**
 * A wrapper around a mutexed object
 * @tparam T the object type
*/
template <typename T>
class locked
{
public:
	locked() = default;
	// locked(T&& x) : mt(), data(x) {}

	/**
	 * 
	*/
	template <typename Func>
	auto operate(Func f)
	{
		lock_guard<mutex> lock(mt);
		return f(data);
	}

private:
	T data;
	mutex mt;
};