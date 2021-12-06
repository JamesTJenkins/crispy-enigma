#pragma once
#include <glm.hpp>

namespace VulkanModule {
    struct PushConstants {
        glm::mat4 model;
        int texId;
        float specular;
        float shininess;
    };
}