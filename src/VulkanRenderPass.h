#pragma once
#include <vulkan/vulkan.h>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanImageView.h"

namespace VulkanModule {
    class VulkanRenderPass {
    public:
        // Constructors
        VulkanRenderPass(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanImageView* imageView);
        ~VulkanRenderPass();

        // Creates the renderpass
        void CreateRenderPass();

        // Cleanup
        void Cleanup();

        // Variables
        VkRenderPass renderPass;
    private:
        // References
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanImageView* vImageView;
    };
}