#pragma once

#define GLM_FORCE_RADIANS
#define GLM_RIGHT_HANDED

#include <glm.hpp>
#include <gtx/quaternion.hpp>

namespace Components {
    struct Transform {
    public:
        Transform(glm::vec3 position = glm::vec3(0,0,0), glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1,1,1));
        Transform(glm::mat4 matrix);

	    glm::mat4 transform;

        void SetPosition(glm::vec3 position);
        void SetRotation(glm::quat rotation);
        void SetScale(glm::vec3 scale);

        void Rotate(float angle_in_radians, glm::vec3 axis);
        void Rotate(glm::quat quaternion);

        // Converts euler angles to quaterion
        glm::quat EulerToQuat(glm::vec3 rotation_in_degrees);
        // Converts quaternion to euler angles, returns in degrees
        glm::vec3 QuatToEuler(glm::quat quaternion);

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
        void DecomposeMatrix();

        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
    };
}
