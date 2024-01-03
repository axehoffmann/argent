#pragma once

#include "lib/vector.h"
#include "glm.h"

/**
 * Per-instance render data
*/
struct alignas(16) instance_data
{
	glm::mat4 model;
	glm::vec4 boundingSphere;
	u32 mat;
	u32 mesh;
};

struct scene_info
{
	u32 totalObjects;

	glm::mat4 view;
	f32 frustum[4];
};

struct scene_graph
{
	scene_info info;
	vector<instance_data> scene;
	vector<u32> meshCounts;
};