#pragma once
#include <glm.hpp>

namespace VulkanModule {
    struct PushConstants {
        // Currently takes 76 bytes out the max 128 bytes

        glm::mat4 model;
        int texId;
        float specular;
        float shininess;
    };
}