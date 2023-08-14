#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
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

            glm::mat4 out = glm::translate(glm::mat4(1.0f), tr.GetPosition());
            //out = glm::rotate(out, rot.w, glm::vec3(rot.x, rot.y, rot.z));
            out = (tr.GetRotation().operator glm::mat<4, 4, glm::f32, glm::packed_highp>()) * out;
            out = glm::scale(out, tr.GetScale());
            Log::Trace(sfmt("{} {} {} {}", rot.x, rot.y, rot.z, rot.w));


            return out;

        }

        static glm::mat4 ProjectionMatrix(const ag::Camera& camera)
		{
			return glm::perspective(glm::radians(camera.fieldOfView), camera.aspectRatio, camera.nearPlane, camera.farPlane);
        }
        
        static glm::mat4 ViewMatrix(const ag::Transform& transform)
		{
			return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.Front(), transform.Up());
		}
    };
}