#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <nlohmann/json.hpp>

#include "ecs/Component.h"

namespace ag
{
	struct PointLight
	{
	public:
		PointLight(glm::vec3 pos, glm::vec4 col, float intense) : position(pos), colour(col), intensity(intense) { }
		glm::vec3 position;
		glm::vec4 colour;
		float intensity;
	};

	struct PointLightComp
	{
		glm::vec4 colour;

		static nlohmann::json ToJSON(const PointLightComp& t)
		{
			using json = nlohmann::json;
			json ob{
				{ "type", "pointLight" },
				{ "colour", { t.colour.x, t.colour.y, t.colour.z, t.colour.w } }
			};
			return ob;
		}
		static PointLightComp FromJSON(nlohmann::json& ob)
		{
			using json = nlohmann::json;
			json c = ob["colour"];
			return PointLightComp{ { c[0], c[1], c[2], c[3] } };
		}

	private:
		static inline auto serialiser = ag::Component::Serialiser<PointLightComp>("pointLight");
	};
}