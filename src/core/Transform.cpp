#include "Transform.h"

ag::Transform::Transform(glm::vec3 pos)
{
    position = pos;
    rotation = glm::quat();
    scale = glm::vec3(1.0f);
    hasChanged = false;
}

ag::Transform::Transform(glm::vec3 pos, glm::vec3 rot)
{
    position = pos;
    rotation = glm::quat(rot);
    scale = glm::vec3(1.0f);
    hasChanged = false;
}

ag::Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc)
{
    position = pos;
    rotation = glm::quat(rot);
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

void ag::Transform::SetScale(glm::vec3 sc)
{
    scale = sc;
    hasChanged = true;
}

glm::vec3 ag::Transform::GetPosition()
{
    return position;
}

glm::vec3 ag::Transform::GetRotation()
{
    return glm::eulerAngles(rotation);
}

glm::vec3 ag::Transform::GetScale()
{
    return scale;
}

bool ag::Transform::HasChanged()
{
    return hasChanged;
}

void ag::Transform::Clean()
{
    hasChanged = false;
}

glm::vec3 ag::Transform::Up()
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 ag::Transform::Right()
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 ag::Transform::Front()
{
    return rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}
