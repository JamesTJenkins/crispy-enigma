#pragma once
#include "Transform.h"

namespace Components {
    class Camera {
    public:
        Camera(const Transform* transform, const float aspect, const float fov, const float nearPlane, const float farPlane, const bool isOrthographic);
	
		glm::mat4 GetViewMatrix() const;
		glm::mat4 WorldToCameraMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

		void SwapPerspective();
    private:
        glm::mat4 projectionMatrix;
		// Camera Transform reference
		const Transform* transformComponent;
		// Camera Settings
		bool isOrthagraphic = false;
		float aspect;
		float fov;
		float nearPlane;
		float farPlane;
    };
}