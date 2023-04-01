#pragma once

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../ecs/ECSTypes.h"
#include <vector>

namespace ag
{
    /**
     * Stores 3D transformation data of an entity.
    */
    struct Transform
    {
    public:

        Transform(glm::vec3 pos);
        Transform(glm::vec3 pos, glm::vec3 rot);
        Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc);

        /**
         * Moves the transformation by a given delta.
         * @param delta The vector to move by.
        */
        void Translate(glm::vec3 delta);

        /**
         * Rotates the transformation by a given euler angle.
         * @param delta The angles to rotate by.
        */
        void Rotate(glm::vec3 delta);
        
        void SetPosition(glm::vec3 pos);
        void SetRotation(glm::vec3 rot);
        void SetScale(glm::vec3 sc);

        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();

        /**
         * @return Whether the Transform has changed since the last time Clean() was invoked.
        */
        bool HasChanged();
        /**
         * Marks the Transform as clean until it is changed.
        */
        void Clean();

        glm::vec3 Up();
        glm::vec3 Right();
        glm::vec3 Front();

    private:
        bool hasChanged;

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        // std::vector<EntityID> children;
    };
}