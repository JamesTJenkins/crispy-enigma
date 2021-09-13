#pragma once
#include <vulkan/vulkan.h>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanFrameBuffer.h"
#include "VulkanImageView.h"
#include "VulkanImage.h"

namespace VulkanModule {
    class VulkanResources {
    public:
        // Constructors
        VulkanResources(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanFrameBuffer* framebuffer, VulkanImageView* imageView, VulkanImage* image);
        ~VulkanResources();

        // Create color resources
        void CreateColorResources();
        // Create depth resources
        void CreateDepthResources();
    private:
        // References
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanFrameBuffer* vFramebuffer;
        VulkanImageView* vImageView;
        VulkanImage* vImage;
    };
}