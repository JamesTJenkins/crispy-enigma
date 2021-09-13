#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"
#include "Root.fwd.h"

namespace VulkanModule {
    class VulkanFrameBuffer {
    public:
        // Constructors
        VulkanFrameBuffer(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanImageView* imageView, VulkanRenderPass* renderpass);
        ~VulkanFrameBuffer();

        // Create framebuffers
        void CreateFramebuffers();
        // Framebuffer resized
        void FramebufferResized();

        // Cleanup
        void Cleanup();

        // Variables
        std::vector<VkFramebuffer> swapchainFramebuffers;
        
        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        bool framebufferResized = false;
    private:
        // References
        Root* root;
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanImageView* vImageView;
        VulkanRenderPass* vRenderpass;
    };
}