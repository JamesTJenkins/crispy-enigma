#include "Transform.h"
#include <gtx/transform.hpp>
#include <gtx/matrix_decompose.hpp>
#include <iostream>

namespace Components {
    Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
        SetPosition(position);
        SetRotation(rotation);
        SetScale(scale);
    }

    void Transform::Rotate(float angle_in_radians, glm::vec3 axis) {
        axis *= sin(angle_in_radians / 2.0f);
        rotation = glm::normalize(glm::quat (cos(angle_in_radians / 2.0f), axis) * rotation);
    }

    glm::quat Transform::EulerToQuat(glm::vec3 rotation_in_degrees) {
       return glm::quat(glm::radians(rotation_in_degrees));
    }

    glm::vec3 Transform::QuatToEuler(glm::quat quaternion) {
        return glm::eulerAngles(quaternion);
    }

    void Transform::SetPosition(glm::vec3 position) { this->position = position; }
    void Transform::SetRotation(glm::quat rotation) { this->rotation = rotation; }
    void Transform::SetScale(glm::vec3 scale) { this->scale = scale; }

	glm::vec3 Transform::GetPosition() const { return position; }
    glm::quat Transform::GetRotation() const { return rotation; }
    glm::vec3 Transform::GetScale() const { return scale; }

    glm::vec3 Transform::Left() const { return rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f); }
    glm::vec3 Transform::Right() const { return rotation * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f); }
    glm::vec3 Transform::Up() const { return rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f); }
    glm::vec3 Transform::Down() const { return rotation * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f); }
    glm::vec3 Transform::Forward() const { return rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); }
    glm::vec3 Transform::Backward() const { return rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); }

    const glm::mat4& Transform::LocalToWorldMatrix() {
        glm::mat4 translationMatrix = glm::translate(position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(scale);
        localToWorldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        return localToWorldMatrix;
    }

    const glm::mat4& Transform::WorldToLocalMatrix() {
        glm::mat4 translationMatrix = glm::translate(-position);
        //glm::mat4 rotationMatrix = glm::mat4_cast(-rotation);
        glm::mat4 rotationMatrix = glm::mat4_cast(glm::inverse(rotation));
        glm::mat4 scaleMatrix = glm::scale(1.0f / scale);
        worldToLocalMatrix = scaleMatrix * rotationMatrix * translationMatrix;

        return worldToLocalMatrix;
    }
}