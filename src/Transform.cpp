#include "Transform.h"

namespace Components {
    Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
        SetPosition(position);
        SetRotation(rotation);
        SetScale(scale);
    }

    void Transform::SetPosition(glm::vec3 position) {
        translationMatrix = glm::mat4(
            1.0f, 0.0f, 0.0f, position.x,
            0.0f, 1.0f, 0.0f, position.y,
            0.0f, 0.0f, 1.0f, position.z,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        RecreateTransformMatrix();
    }

    void Transform::SetRotation(glm::quat rotation) {
        rotationMatrix = glm::mat4_cast(rotation);
        RecreateTransformMatrix();
    }

    void Transform::SetScale(glm::vec3 scale) {
        translationMatrix = glm::mat4(
            scale.x, 0.0f, 0.0f, 0.0f,
            0.0f, scale.y, 0.0f, 0.0f,
            0.0f, 0.0f, scale.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        RecreateTransformMatrix();
    }

	glm::vec3 Transform::GetPosition() {
        return glm::vec3(transform[3]);
    }

    glm::quat Transform::GetRotation() {
        return glm::quat_cast(transform);
    }

    glm::vec3 Transform::GetScale() {
        glm::vec3 scale;
        scale.x  = glm::length(glm::vec3(transform[0]));
        scale.y  = glm::length(glm::vec3(transform[1]));
        scale.z  = glm::length(glm::vec3(transform[2]));
        return scale;
    }

    glm::vec3 Transform::Up() {
        return GetRotation() * glm::vec3(0,1,0);
    }

    glm::vec3 Transform::Down(){
        return GetRotation() * glm::vec3(0,-1,0);
    }

    glm::vec3 Transform::Left(){
        return GetRotation() * glm::vec3(1,0,0);
    }

    glm::vec3 Transform::Right(){
        return GetRotation() * glm::vec3(-1,0,0);
    }

    glm::vec3 Transform::Forward(){
        return GetRotation() * glm::vec3(0,0,1);
    }

    glm::vec3 Transform::Backward(){
        return GetRotation() * glm::vec3(0,0,-1);
    }

    void Transform::RecreateTransformMatrix() {
        transform = translationMatrix * rotationMatrix * scaleMatrix;
    }
}