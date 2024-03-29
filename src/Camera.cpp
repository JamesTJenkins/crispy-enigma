#include "Camera.h"

namespace Components {
	Camera::Camera(const float aspect, const float fov, const float nearPlane, const float farPlane, const bool isOrthographic, Transform* transform) : aspect(aspect), fov(fov), nearPlane(nearPlane), farPlane(farPlane), isOrthographic(isOrthographic), transformComponent(transform){
		CreateCamera();
	}

	glm::mat4 Camera::GetViewMatrix() const {
		return glm::lookAt(transformComponent->GetPosition(), transformComponent->GetPosition() + transformComponent->Forward(), transformComponent->Up());
	}

	glm::mat4 Camera::WorldToCameraMatrix() const {
		return projectionMatrix * glm::lookAt(transformComponent->GetPosition(), transformComponent->GetPosition() + transformComponent->Forward(), transformComponent->Up());
	}

	glm::mat4 Camera::GetProjectionMatrix() const {
		return projectionMatrix;
	}

	void Camera::CreateCamera() {
		if (!isOrthographic) {
			projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
        } else {
			projectionMatrix = glm::ortho(fov, aspect, nearPlane, farPlane);
		}
	}
}