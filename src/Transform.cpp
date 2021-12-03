#include "Transform.h"
#include <gtx/transform.hpp>
#include <gtx/matrix_decompose.hpp>

namespace Components {
    Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
        SetPosition(position);
        SetRotation(rotation);
        SetScale(scale);
    }

    Transform::Transform(glm::mat4 matrix) {
        transform = matrix;
    }

    void Transform::SetPosition(glm::vec3 position) {
        translationMatrix = glm::translate(position);
        /*
        translationMatrix = glm::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
           position.x, position.y, position.z, 1.0f
        );*/

        RecreateTransformMatrix();
    }

    void Transform::SetRotation(glm::quat rotation) {
        rotationMatrix = glm::mat4_cast(rotation);
    
        RecreateTransformMatrix();
    }

    void Transform::SetScale(glm::vec3 scale) {
        scaleMatrix = glm::scale (scale);
        /*
        scaleMatrix = glm::mat4(
            scale.x, 0.0f, 0.0f, 0.0f,
            0.0f, scale.y, 0.0f, 0.0f,
            0.0f, 0.0f, scale.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        */

        RecreateTransformMatrix();
    }

    void Transform::Rotate(float angle_in_radians, glm::vec3 axis) {
        rotationMatrix = glm::rotate(rotationMatrix, angle_in_radians, axis);

        RecreateTransformMatrix();
    }

    void Transform::Rotate(glm::quat quaternion) {
        rotationMatrix = glm::mat4_cast(GetRotation() * quaternion);
        
        RecreateTransformMatrix();
    }

    glm::quat Transform::EulerToQuat(glm::vec3 rotation_in_degrees) {
       return glm::quat(glm::radians(rotation_in_degrees));
    }

    glm::vec3 Transform::QuatToEuler(glm::quat quaternion) {
        return glm::eulerAngles(quaternion);
    }

	glm::vec3 Transform::GetPosition() {
        return glm::vec3(translationMatrix[3]);
    }

    glm::quat Transform::GetRotation() {
        return glm::quat_cast(rotationMatrix);
    }

    glm::vec3 Transform::GetScale() {
        glm::vec3 scale;
        scale.x  = glm::length(glm::vec3(scaleMatrix[0]));
        scale.y  = glm::length(glm::vec3(scaleMatrix[1]));
        scale.z  = glm::length(glm::vec3(scaleMatrix[2]));
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

    void Transform::DecomposeMatrix() {
        // Used purely to create the other matricies if transform was created with a matrix
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 skew;
        glm::vec4 perspective;

        // Decompose
        glm::decompose(transform, scale, rotation, translation, skew, perspective);
    
        // Fill required values
        SetPosition(translation);
        SetRotation(rotation);
        SetScale(scale);
    }

    void Transform::RecreateTransformMatrix() {
        transform = translationMatrix * rotationMatrix * scaleMatrix;
    }
}