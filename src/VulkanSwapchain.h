#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "Root.fwd.h"

namespace VulkanModule {
    class VulkanSwapchain {
    public:
        VulkanSwapchain(Root* _root, VulkanDevice* _vulkanDevice, VulkanSurface* _vulkanSurface);
        ~VulkanSwapchain();

        // Create swapchain
        void CreateSwapchain();

        // Cleanup
        void Cleanup();

        // Helper functions

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int windowWidth, int windowHeight);

        // Variables
        VkSwapchainKHR swapchain;
        std::vector<VkImage> swapchainImages;
        VkFormat swapchainImageFormat;
        VkExtent2D swapchainExtent;

    private:
        Root* root;
        VulkanDevice* vDevice;
        VulkanSurface* vSurface;
    };
}