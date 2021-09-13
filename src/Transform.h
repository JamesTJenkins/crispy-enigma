#pragma once

#define GLM_FORCE_RADIANS

#include <glm.hpp>
#include <gtx/quaternion.hpp>

namespace Components {
    struct Transform {
    public:
        Transform(glm::mat4 _transform = glm::mat4(0.0f));

	    glm::mat4 transform;

        glm::vec3 GetPosition();
        glm::quat GetRotation();
        glm::vec3 GetScale();

        glm::vec3 Up();
        glm::vec3 Down();
        glm::vec3 Left();
        glm::vec3 Right();
        glm::vec3 Forward();
        glm::vec3 Backward();
    };
}
