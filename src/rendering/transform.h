#pragma once

#include "rendering/glm.h"
#include "ecs/Component.h"

#include "lib/basic.h"

#include <format>

template <>
struct std::formatter<glm::vec3>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin();
	}

	auto format(const glm::vec3& ob, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "({}, {}, {})", ob.x, ob.y, ob.z);
	}
};


template <>
struct std::formatter<glm::vec4>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin();
	}

	auto format(const glm::vec4& ob, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "({}, {}, {}, {})", ob.x, ob.y, ob.z, ob.w);
	}
};


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