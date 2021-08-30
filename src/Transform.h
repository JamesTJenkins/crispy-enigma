#pragma once
#include <gtx/quaternion.hpp>

namespace Components {
    struct Transform {
    public:
		Transform(const glm::vec3 position = glm::vec3(0,0,0), const glm::quat rotation = glm::quat(), const glm::vec3 scale = glm::vec3(1,1,1));

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

		glm::mat4 LocalToWorldMatrix() const;

		void Rotate(const glm::vec3& angles);
		void Rotate(const float x, const float y, const float z);
		void RotateOnAxis(const float& angle, const glm::vec3& axis);

        glm::vec3 Left() const;
		glm::vec3 Right() const;
		glm::vec3 Up() const;
		glm::vec3 Down() const;
		glm::vec3 Forward() const;
		glm::vec3 Back() const;
		glm::vec3 GetEulerAngles() const;

    private:
		glm::vec3 eulerAngles;
    };
}
