#pragma once

#include <vector>
#include <array>
#include "basic.h"

template <typename T, u64 size>
using arr = std::array<T, size>;

template <typename T>
using vector = std::vector<T>;

template <typename T>
using small_vector = std::vector<T>;


template <typename T>
struct range
{
	constexpr range(vector<T> arr) : ptr(arr.data()), size(arr.size()) {}
	constexpr range(T* data, u64 len) : ptr(data), size(len) {}

	constexpr T* begin() const noexcept
	{
		return ptr;
	}

	constexpr T* end() const noexcept
	{
		return ptr + size;
	}

	constexpr u64 width() const noexcept
	{
		return size;
	}

	range(const range<T>& other) = delete;
	range<T>& operator=(const range<T>& other) = delete;

	constexpr range(range<T>&& other) noexcept : ptr(other.ptr), size(other.size)
	{
		other.ptr = nullptr;
		other.size = 0;
	}
	
	constexpr range<T>& operator=(range<T>&& other) noexcept
	{
		std::swap(ptr, other.ptr);
		std::swap(size, other.size);
	}


private:
	T* ptr {nullptr};
	u64 size {0};
};