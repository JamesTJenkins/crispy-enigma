#pragma once
#include "Transform.h"

namespace Components {
    enum LightType { DIRECTIONAL = 0, POINT = 1 };

    struct Light {
    public:
        Light(const LightType _lightType, const glm::vec3 _position, const glm::vec3 lightColor, const float lightRange);

        // Light settings
        LightType lightType;
        glm::vec3 position;
        glm::vec3 color;
        float radius;
    private:

    };

    struct LightData {
        LightData(const glm::vec4 vector = glm::vec4(0,0,0,0), const glm::vec3 color = glm::vec3(0,0,0), const float radius = 0.0f);

        glm::vec4 position;
        alignas(16) glm::vec3 color;
        float radius;
    };
}