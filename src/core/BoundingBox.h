#pragma once

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>

namespace ag
{
    struct BoundingBox
    {
    public:
        BoundingBox(glm::vec3 pos, glm::vec3 ext);

        // bool Overlaps(BoundingBox other);
        bool ContainsPoint(glm::vec3 point);
    private:
        glm::vec3 origin;
        glm::vec3 extents;
    };
}


