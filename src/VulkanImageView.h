#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

namespace VulkanModule {
    class VulkanImageView {
    public:
        // Constructors
        VulkanImageView(VulkanDevice* device, VulkanSwapchain* swapchain);
        ~VulkanImageView();

        // Creates all required image views
        void CreateImageViews();

        // Creates and returns individual image views
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        // Cleanup
        void Cleanup();

        // Helper functions
        VkFormat FindDepthFormat();
        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        // Variables
        std::vector<VkImageView> swapchainImageViews;
    private:
        // References
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
    };
}