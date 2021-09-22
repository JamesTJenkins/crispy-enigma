#pragma once
#include "Transform.h"

namespace Components {
    struct Camera {
    public:
		Camera(const float aspect, const float fov, const float nearPlane, const float farPlane, const bool isOrthographic, Transform* transform);

		// Camera Settings
		float aspect;
		float fov;
		float nearPlane;
		float farPlane;
		bool isOrthographic = false;
		// Camera Transform reference
		Transform* transformComponent;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 WorldToCameraMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

		void CreateCamera();
    private:
        glm::mat4 projectionMatrix;
    };
}