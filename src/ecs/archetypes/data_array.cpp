#include "data_array.h"

void ag::data_array::insert(byte* data, u32 bytes)
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

void ag::data_array::eraseIndices(const dyn_array<u64>& indices)
{
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
			if (predicate()) return start;
			pos += componentSize;
		}
	}();

	// std::remove_if with variable-size steps
	if (first == end)
	{
		for (auto it = first; (it += componentSize) != end;)
		{
			if (!predicate())
			{
				first += componentSize;
				std::memcpy(first, it, componentSize);
			}
		}
	}

	end = first;
}