#pragma once

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../ecs/ECSTypes.h"
#include "../ecs/Component.h"
#include <vector>

namespace ag
{
    /// TODO: should we have a system where transforms/other components can all share a reference to a position value etc?


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
        void SetRotation(glm::quat rot);
        void SetScale(glm::vec3 sc);

        glm::vec3 GetPosition();
        glm::quat GetRotation();
        glm::vec3 GetEulerRotation();
        glm::vec3 GetScale();

        /**
         * @return Whether the Transform has changed since the last time Clean() was invoked.
        */
        bool HasChanged();
        /**
         * Marks the Transform as clean until it is changed.
        */
        void Clean();

        /**
        * Returns the up vector relative to the transform's rotation
        */
        glm::vec3 Up();
        /**
        * Returns the right vector relative to the transform's rotation
        */
        glm::vec3 Right();
        /**
        * Returns the forward vector relative to the transform's rotation
        */
        glm::vec3 Front();

        static nlohmann::json ToJSON(Transform t);
        static Transform FromJSON(nlohmann::json& ob);

    private:
        static ag::Component::Serialiser<Transform> serialiser;

        bool hasChanged;

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        // std::vector<EntityID> children;
    };
}