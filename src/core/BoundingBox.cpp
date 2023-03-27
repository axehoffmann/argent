#include "BoundingBox.h"

ag::BoundingBox::BoundingBox(glm::vec3 pos, glm::vec3 ext)
{
    origin = pos;
    extents = ext;
}

bool ag::BoundingBox::ContainsPoint(glm::vec3 point)
{
    bool x = point.x > origin.x && point.x < (origin.x + extents.x);
    bool y = point.y > origin.y && point.y < (origin.y + extents.y);
    bool z = point.z > origin.z && point.z < (origin.z + extents.z);
    return x && y && z;
}