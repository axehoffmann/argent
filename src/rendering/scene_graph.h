#pragma once

#include "lib/vector.h"
#include "transform.h"

/**
 * Per-instance render data
*/
struct instance_data
{
	glm::mat4 model;
	u32 mat;
	u32 mesh;
	ag::byte padding[8];
};

struct scene_info
{
	u32 totalObjects;
};

struct scene_graph
{
	scene_info info;
	vector<instance_data> scene;
	vector<u32> meshCounts;
};