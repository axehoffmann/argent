#pragma once

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

#include "../ecs/ECSTypes.h"
#include <vector>

namespace ag
{
    struct Transform
    {
    public:
        Transform(glm::vec3 pos);
        Transform(glm::vec3 pos, glm::vec3 rot);
        Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc);

        void Translate(glm::vec3 delta);
        void Rotate(glm::vec3 delta);
        
        void SetPosition(glm::vec3 pos);
        void SetRotation(glm::vec3 rot);
        void SetScale(glm::vec3 sc);

        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();

        bool HasChanged();
        void Clean();

        glm::vec3 Up();
        glm::vec3 Right();
        glm::vec3 Front();

    private:
        std::vector<EntityID> children;
        bool hasChanged;

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    };
}