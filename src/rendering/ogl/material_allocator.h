#pragma once

#include <cmath>

#include "lib/basic.h"
#include "lib/vector.h"

#include "buffer.h"
#include "texture.h"

// 2^18 = 262,144 maximum materials
// 2^18 * 4B index = 1MiB
/// TODO: this should actually change automatically based on asset pack size probably.
constexpr u32 max_materials = 1 << 18;

constexpr u32 max_material_cache = 1024;

struct gl_material
{
	u64 alb;
	u64 norm;
	u64 detail;
	u64 detail2;
};

class material_allocator
{
private:
	struct resident_material
	{
		gl_material mat;

		u32 materialID;

		u32 pad;
		ag::byte padding[6];
	};

public:
	material_allocator() :
		gpu_allocation_table(buffer_access_type::DynamicDraw, buffer_type::Storage),
		gpu_resident_materials(buffer_access_type::DynamicDraw, buffer_type::Storage)
	{
		gpu_allocation_table.allocate(sizeof(u32) * max_materials);
		gpu_resident_materials.allocate(sizeof(resident_material) * max_material_cache);
	}

	void evict(u32 material)
	{
		u64 materialLoc = allocation_table[material];
		
		// Material is not resident
		if (materialLoc == 0) return;

		// Swap with the last resident material to erase
		u32 replacementID = resident_materials[resident_material_count - 1].materialID;
		resident_materials[materialLoc - 1] = std::move(resident_materials[resident_material_count - 1]);
		allocation_table[material] = 0;
		allocation_table[replacementID] = materialLoc;
		resident_material_count--;

		/// TODO: when evicting, unload textures from GPU memory
	}

	void load(gl_material descriptor, u32 material)
	{
		u32 currentLoc = allocation_table[material];

		// Material already loaded
		if (currentLoc != 0) return;

		/// TODO: what to do on cache full
		// Append to the cache
		resident_materials[resident_material_count] = resident_material{ descriptor, material };
		allocation_table[material] = resident_material_count;
		resident_material_count++;
	}

private:
	
	
	arr<u32, max_materials> allocation_table;
	buffer gpu_allocation_table;

	arr<resident_material, max_material_cache> resident_materials;
	buffer gpu_resident_materials;
	u64 resident_material_count;
};