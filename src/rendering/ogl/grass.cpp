#include "grass.h"

#include "lib/vector.h"
#include "rendering/glm.h"

struct grass_info
{
	glm::vec4 pos;
};

buffer generateGrassBuffer()
{
	const f32 dist = 3;
	const f32 begin = -dist / 2.0f;
	const f32 density = 1.0f;

	const f32 spacing = 1.0f / density;

	buffer bf{ buffer_access_type::DynamicDraw, buffer_type::Storage };

	const u32 bladeCount = u32((dist / spacing) * (dist / spacing));
	vector<grass_info> grass(bladeCount);

	for (u32 i = 0; i < bladeCount; i++)
	{
		glm::vec4 pos{ begin + (i / u32(dist / spacing)) * spacing, 0.0f, begin + (i % u32(dist / spacing)) * spacing, 0.0f };

		grass[i] = grass_info{ pos };
	}

	bf.setData(grass.data(), sizeof(grass_info) * grass.size());

	return bf;
}