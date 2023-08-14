#include "Transform.h"

ag::Component::Serialiser<ag::Transform> ag::Transform::serialiser("transform");


ag::Transform::Transform(glm::vec3 pos)
{
    position = pos;
    rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    hasChanged = false;
}

ag::Transform::Transform(glm::vec3 pos, glm::vec3 rot)
{
    position = pos;
    rotation = glm::quat(glm::radians(rot));
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    hasChanged = false;
}

ag::Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc)
{
    position = pos;
    rotation = glm::quat(glm::radians(rot));
    scale = sc;
    hasChanged = false;
}

void ag::Transform::Translate(glm::vec3 delta)
{
    position += delta;
    hasChanged = true;
}

void ag::Transform::Rotate(glm::vec3 delta)
{
    rotation *= glm::quat(delta);
    hasChanged = true;
}

void ag::Transform::SetPosition(glm::vec3 pos)
{
    position = pos;
    hasChanged = true;
}

void ag::Transform::SetRotation(glm::vec3 rot)
{
    rotation = glm::quat(rot);
    hasChanged = true;
}

void ag::Transform::SetRotation(glm::quat rot)
{
    rotation = rot;
    hasChanged = true;
}

void ag::Transform::SetScale(glm::vec3 sc)
{
    scale = sc;
    hasChanged = true;
}

glm::vec3 ag::Transform::GetPosition() const
{
    return position;
}

glm::quat ag::Transform::GetRotation() const
{
    return rotation;
}

glm::vec3 ag::Transform::GetEulerRotation() const
{
    return glm::eulerAngles(rotation);
}

glm::vec3 ag::Transform::GetScale() const
{
    return scale;
}

bool ag::Transform::HasChanged() const
{
    return hasChanged;
}

void ag::Transform::Clean()
{
    hasChanged = false;
}

glm::vec3 ag::Transform::Up() const
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 ag::Transform::Right() const
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 ag::Transform::Front() const
{
    return rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

nlohmann::json ag::Transform::ToJSON(Transform t)
{
    using json = nlohmann::json;
    glm::vec3 rot = glm::eulerAngles(t.rotation);
    json ob = {
        { "type", "transform" },
        { "position", { t.position.x, t.position.y, t.position.z } },
        { "rotation", { rot.x, rot.y, rot.z} },
        { "scale", { t.scale.x, t.scale.y, t.scale.z } }
    };
    return ob;
}

ag::Transform ag::Transform::FromJSON(nlohmann::json& ob)
{
    using json = nlohmann::json;
    json pos = ob["position"];
    json rot = ob["rotation"];
    json scl = ob["scale"];

    return Transform(
        glm::vec3(pos[0], pos[1], pos[2]),
        glm::vec3(rot[0], rot[1], rot[2]),
        glm::vec3(scl[0], scl[1], scl[2])
    );
}
