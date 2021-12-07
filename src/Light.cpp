#include "Light.h"

namespace Components {
    Light::Light(const LightType _lightType, const glm::vec3 _position, const glm::vec3 lightColor, const float lightRange) : lightType(_lightType), position(_position), color(lightColor), radius(lightRange) {

    }

    LightData::LightData(const glm::vec4 vector, const glm::vec3 color, const float radius) : position(vector), color(color), radius(radius) {

    }
}