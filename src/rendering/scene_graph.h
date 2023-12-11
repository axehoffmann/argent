#pragma once

#include "lib/vector.h"
#include "transform.h"

/**
 * Per-instance render data
*/
struct render_instance
{
	glm::mat4 model;
	u32 mat;

	ag::byte padding[12];
};

struct scene_graph
{
	vector<vector<render_instance>> scene;
};