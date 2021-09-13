#include "Transform.h"

namespace Components {
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
}