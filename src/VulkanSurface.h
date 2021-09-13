#pragma once
#include <vulkan/vulkan.h>

#include "VulkanInstance.h"
#include "Root.fwd.h"

namespace VulkanModule {
    class VulkanSurface {
    public:
        VulkanSurface(Root* _root, VulkanInstance* _vulkanInstance);
        ~VulkanSurface();

        // Creates a surface
        void CreateSurface();

        // Cleanup
        void Cleanup();

        // Variables
        VkSurfaceKHR surface;

    private:
        Root* root;
        VulkanInstance* vInstance;
    };
}