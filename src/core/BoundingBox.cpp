#include "BoundingBox.h"

ag::BoundingBox::BoundingBox(glm::vec3 pos, glm::vec3 ext)
{
    origin = pos;
    extents = ext;
}

glm::vec3 ag::BoundingBox::GetOrigin()
{
    return origin;
}

glm::vec3 ag::BoundingBox::GetExtents()
{
    return extents;
}

glm::vec3 ag::BoundingBox::GetCenter()
{
    return center;
}

void ag::BoundingBox::SetOrigin(glm::vec3 pos)
{
    origin = pos;
    center = origin + 0.5f * extents;
}

void ag::BoundingBox::SetExtents(glm::vec3 ext)
{
    extents = ext;
    center = origin + 0.5f * extents;

}

bool ag::BoundingBox::ContainsPoint(glm::vec3 point)
{
    bool x = point.x > origin.x && point.x < (origin.x + extents.x);
    bool y = point.y > origin.y && point.y < (origin.y + extents.y);
    bool z = point.z > origin.z && point.z < (origin.z + extents.z);
    return x && y && z;
}