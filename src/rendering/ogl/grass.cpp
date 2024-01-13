#include "grass.h"

#include "lib/vector.h"
#include "rendering/glm.h"

struct grass_info
{
	glm::vec4 pos;
};

buffer generateGrassBuffer()
{
	const f32 dist = 45;
	const f32 begin = -dist / 2.0f;
	const f32 density = 10.0f;

	const f32 spacing = 1.0f / density;

	buffer bf{ buffer_access_type::DynamicDraw, buffer_type::Storage };

	const u32 perUnit = u32((dist / spacing));
	const u32 bladeCount = perUnit * perUnit;
	vector<grass_info> grass(bladeCount);

	for (u32 i = 0; i < bladeCount; i++)
	{
		f32 row = (i % perUnit);
		f32 col = (i / perUnit);
		glm::vec4 pos{ begin + row * spacing, -2.0f, begin + col * spacing - 10.0f, 0.0f };

		grass[i] = grass_info{ pos };
	}

	bf.setData(grass.data(), sizeof(grass_info) * grass.size());

	return bf;
}