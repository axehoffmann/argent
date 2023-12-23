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

nlohmann::json transform::ToJSON(const transform& t)
{
    using json = nlohmann::json;
    glm::vec3 rot = glm::eulerAngles(t.rot);
    json ob = {
        { "type", "transform" },
        { "position", { t.pos.x, t.pos.y, t.pos.z } },
        { "rotation", { rot.x, rot.y, rot.z} },
        { "scale", { t.scale.x, t.scale.y, t.scale.z } }
    };
    return ob;
}

transform transform::FromJSON(nlohmann::json& ob)
{
    using json = nlohmann::json;
    json pos = ob["position"];
    json rot = ob["rotation"];
    json scl = ob["scale"];

    return transform{
        glm::vec3(pos[0], pos[1], pos[2]),
        glm::quat(glm::vec3(rot[0], rot[1], rot[2])),
        glm::vec3(scl[0], scl[1], scl[2])
    };
}