#pragma once

#include "rendering/glm.h"
#include "ecs/Component.h"

#include "lib/basic.h"

struct transform
{
	glm::vec3 pos;
	glm::quat rot;
	glm::vec3 scale;

	glm::vec3 front() const
	{
		return rot * glm::vec3(0.0f, 0.0f, 1.0f);
	}

	glm::vec3 up() const
	{
		return rot * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::vec3 right() const
	{
		return rot * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	static inline auto serialiser = ag::Component::Serialiser<transform>("transform");

	static nlohmann::json ToJSON(const transform& t);
	static transform FromJSON(nlohmann::json& ob);
};

glm::mat4 model_matrix(const transform& t);

glm::mat4 view_matrix(const transform& t);

glm::mat4 projection_matrix(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);