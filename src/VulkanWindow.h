#pragma once
#include "Root.fwd.h"

namespace VulkanModule {
    class VulkanWindow {
    public:
        // Constructors
        VulkanWindow(Root* _root);
        ~VulkanWindow();

        // Create sdl2 window with required vulkan flags
        void CreateVulkanWindow(const char *title);
    private:
        // References
        Root* root;
    };
}