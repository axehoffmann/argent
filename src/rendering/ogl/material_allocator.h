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
public:
	material_allocator() :
		allocation_table(),
		gpu_allocation_table(buffer_access_type::DynamicDraw, buffer_type::Storage),
		resident_materials(),
		gpu_resident_materials(buffer_access_type::DynamicDraw, buffer_type::Storage),
		resident_material_ids(),
		resident_material_count(0)
	{
		gpu_allocation_table.allocate(sizeof(u32) * max_materials);
		gpu_allocation_table.bind();
		gpu_allocation_table.bind(3);
		
		gpu_resident_materials.allocate(sizeof(gl_material) * max_material_cache);
		gpu_resident_materials.bind();
		gpu_resident_materials.bind(2);
	}

	void evict(u32 material)
	{
		u32 materialLoc = allocation_table[material];
		
		// Material is not resident
		if (materialLoc == 0) return;

		u64 idx = static_cast<u64>(materialLoc) - 1;

		/// TODO: we probably don't need to mirror half of this on the cpu. Can probably do a full GPU-side replace
		// Swap with the last resident material to erase
		u32 replacementID = resident_material_ids[resident_material_count - 1];
		resident_material_ids[idx] = replacementID;
		resident_materials[idx] = std::move(resident_materials[resident_material_count - 1]);
		allocation_table[material] = 0;
		allocation_table[replacementID] = materialLoc;
		resident_material_count--;
		
		// Mirror the action on the GPU
		u32 erasure = 0;
		gpu_resident_materials.set(&resident_materials[resident_material_count - 1], sizeof(gl_material), sizeof(gl_material) * idx);
		
		gpu_allocation_table.set(&erasure, sizeof(u32), sizeof(u32) * material);
		gpu_allocation_table.set(&materialLoc, sizeof(u32), sizeof(u32) * replacementID);
		
		/// TODO: when evicting, unload textures from GPU memory
	}

	void load(gl_material descriptor, u32 material)
	{
		u32 currentLoc = allocation_table[material];

		// Material already loaded
		if (currentLoc != 0) return;

		u32 newLoc = static_cast<u32>(resident_material_count) + 1;

		// Append to the cache
		resident_materials[resident_material_count] = descriptor;
		resident_material_ids[resident_material_count] = material;
		allocation_table[material] = newLoc;

		for (size_t i = 0; i < 2; i++)
		{
			ag::Log::Trace(ag::sfmt("{}   {}", newLoc, material));
		}

		// Mirror the action on the GPU
		gpu_resident_materials.set(&descriptor, sizeof(gl_material), sizeof(gl_material) * resident_material_count);
		gpu_allocation_table.set(&newLoc, sizeof(u32), sizeof(u32) * material);

		resident_material_count++;
	}

private:
	
	
	arr<u32, max_materials> allocation_table;
	buffer gpu_allocation_table;

	arr<gl_material, max_material_cache> resident_materials;
	buffer gpu_resident_materials;
	// Reverse-direction reference to help upkeep the cache lookup table
	arr<u32, max_material_cache> resident_material_ids;
	u64 resident_material_count;
};