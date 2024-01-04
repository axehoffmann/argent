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
	glm::mat4 view;
	u32 totalObjects;

	f32 frustum[4];
};

struct scene_graph
{
	scene_info info;
	vector<instance_data> scene;
	vector<u32> meshCounts;
};