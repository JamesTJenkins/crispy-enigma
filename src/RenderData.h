#pragma once
#include <glm.hpp>
#include "MeshRenderer.h"

namespace VulkanModule {
    struct RenderData {
        Components::MeshRenderer* meshRenderer;
        glm::mat4 transform;
    };
}