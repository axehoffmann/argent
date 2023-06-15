#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ag
{
	struct PointLight
	{
	public:
		PointLight(glm::vec3* pos, glm::vec3 col, float intense) : position(pos), colour(col), intensity(intense) { }
		glm::vec3* position;
		glm::vec3 colour;
		float intensity;
	};
}