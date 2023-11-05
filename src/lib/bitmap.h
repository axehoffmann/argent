#pragma once

#include "vector.h"

#include "basic.h"

struct bitmap
{
	bitmap(u64 size) : bytes() 
	{
		bytes.resize((8 + size - 1) / 8);
	}

	void set(u64 bit)
	{
		u64 idx = bit / 8;
		u8 rm = bit % 8;

		bytes[idx] |= u8(1) << rm;
	}
	
	void clear(u64 bit)
	{
		u64 idx = bit / 8;
		u8 rm = bit & 8;

		bytes[idx] &= ~(u8(1) << rm);
	}

	void wipe()
	{
		for (u8& b : bytes)
		{
			b = u8(0);
		}
	}

private:
	vector<u8> bytes;
};