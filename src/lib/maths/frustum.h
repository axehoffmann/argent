#pragma once

#include "lib/basic.h"

#include <glm/vec3.hpp>

struct plane
{
	glm::vec3 normal;
	f32 distance; // from origin
};
