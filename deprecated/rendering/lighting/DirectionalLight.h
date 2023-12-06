#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ag
{
	struct DirectionalLight
	{
	public:
		DirectionalLight(glm::vec3* dir, glm::vec3 col, float intense) : direction(dir), colour(col), intensity(intense) { }
		glm::vec3* direction;
		glm::vec3 colour;
		float intensity;
	};
}