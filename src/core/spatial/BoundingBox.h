#pragma once

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>

namespace ag
{
    struct BoundingBox
    {
    public:
        BoundingBox(glm::vec3 pos, glm::vec3 ext);

        glm::vec3 GetOrigin();
        glm::vec3 GetExtents();

        glm::vec3 GetCenter();

        void SetOrigin(glm::vec3 pos);
        void SetExtents(glm::vec3 ext);

        // bool Overlaps(BoundingBox other);
        bool ContainsPoint(glm::vec3 point);
    private:
        glm::vec3 origin;
        glm::vec3 extents;

        glm::vec3 center;
    };
}


