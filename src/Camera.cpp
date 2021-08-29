#include "Camera.h"

namespace Components {
	Camera::Camera(const Transform* transform, const float aspect, const float fov, const float nearPlane, const float farPlane, const bool isOrthographic) : aspect(aspect), fov(fov), nearPlane(nearPlane), farPlane(farPlane) {
		if (!isOrthographic) {
			isOrthagraphic = true;
			projectionMatrix = glm::ortho(fov, aspect, nearPlane, farPlane);
		} else {
			transformComponent = transform;
			projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
		}
	}

	glm::mat4 Camera::GetViewMatrix() const {
		return glm::lookAt(transformComponent->position, transformComponent->position + transformComponent->Forward(), transformComponent->Up());
	}

	glm::mat4 Camera::WorldToCameraMatrix() const {
		return projectionMatrix * glm::lookAt(transformComponent->position, transformComponent->position + transformComponent->Forward(), transformComponent->Up());
	}

	glm::mat4 Camera::GetProjectionMatrix() const {
		return projectionMatrix;
	}

	void Camera::SwapPerspective() {
		if (isOrthagraphic) {
			isOrthagraphic = false;
			projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
		} else {
			isOrthagraphic = true;
			projectionMatrix = glm::ortho(fov, aspect, nearPlane, farPlane);
		}
	}
}