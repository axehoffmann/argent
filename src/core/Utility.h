#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

#include "Transform.h"
#include "Camera.h"

namespace ag
{
    struct Utility
    {
        static glm::mat4 ModelMatrix(ag::Transform* tr)
        {
            glm::vec4 rot = tr->GetRotation();

            out = glm::scale(out, tr->GetScale());
            out = glm::rotate(out, rot.w, rot.xyz);
            out = glm::translate(glm::mat4(), tr->GetPosition());

            return out;

        }

        static glm::mat4 ProjectionMatrix(ag::Camera* camera)
		{
			return glm::perspective(glm::radians(camera->fieldOfView), camera->aspectRatio, camera->nearPlane, camera->farPlane);
        }
        
        static glm::mat4 ViewMatrix(ag::Transform* transform)
		{
			return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->Front(), transform->Up());
		}
    };
}