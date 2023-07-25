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
        static glm::mat4 ModelMatrix(const ag::Transform& tr)
        {
            glm::quat rot = tr.GetRotation();

            glm::mat4 out = glm::scale(out, tr.GetScale());
            out = glm::rotate(out, rot.w, glm::vec3(rot.x, rot.y, rot.z));
            out = glm::translate(glm::mat4(), tr.GetPosition());

            return out;

        }

        static glm::mat4 ProjectionMatrix(const ag::Camera& camera)
		{
			return glm::perspective(glm::radians(camera.fieldOfView), camera.aspectRatio, camera.nearPlane, camera.farPlane);
        }
        
        static glm::mat4 ViewMatrix(const ag::Transform& transform)
		{
			return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.Front(), {0, 1, 0});
		}
    };
}