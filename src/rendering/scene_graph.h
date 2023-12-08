#pragma once

#include "lib/vector.h"
#include "transform.h"

/**
 * Per-instance render data
*/
struct render_instance
{
	glm::mat4 model;
};

struct scene_graph
{
	vector<vector<render_instance>> scene;
};