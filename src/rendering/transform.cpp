#include "transform.h"

glm::mat4 model_matrix(const transform& t)
{
	auto mat = glm::translate(glm::mat4(1.0f), t.pos);
	mat = mat * glm::mat4_cast(t.rot);
	return glm::scale(mat, t.scale);
}

glm::mat4 view_matrix(const transform& t)
{
	return glm::lookAt(t.pos, t.front(), t.up());
}

glm::mat4 projection_matrix(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane)
{
	return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}