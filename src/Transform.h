#pragma once

#define GLM_FORCE_RADIANS
#define GLM_RIGHT_HANDED

#include <glm.hpp>
#include <gtx/quaternion.hpp>

namespace Components {
    struct Transform {
    public:
        Transform(glm::vec3 position = glm::vec3(0,0,0), glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1,1,1));

	    glm::mat4 transform;

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::quat rotation);
        void SetScale(glm::vec3 scale);

        glm::vec3 GetPosition();
        glm::quat GetRotation();
        glm::vec3 GetScale();

        glm::vec3 Up();
        glm::vec3 Down();
        glm::vec3 Left();
        glm::vec3 Right();
        glm::vec3 Forward();
        glm::vec3 Backward();
    private:
        void RecreateTransformMatrix();

        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
    };
}
