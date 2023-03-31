#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Transform.h"
#include "Camera.h"

namespace ag
{
    struct Utility
    {
        glm::mat4 ProjectionMatrix(ag::Camera* camera)
		{
			return glm::perspective(glm::radians(camera->fieldOfView), camera->aspectRatio, camera->nearPlane, camera->farPlane);
        }
        
        glm::mat4 ViewMatrix(ag::Transform* transform)
		{
			return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->Front(), transform->Up());
		}
    };
}