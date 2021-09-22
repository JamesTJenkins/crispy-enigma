#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Root.fwd.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"

namespace VulkanModule {
    class VulkanCommand {
    public:
        // Constructor
        VulkanCommand(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanRenderPass* renderpass, VulkanFrameBuffer* framebuffer);
        ~VulkanCommand();

        // Create command pool
        void CreateCommandPool();
        // Create command buffers
        void CreateCommandBuffers(std::vector<VkDescriptorSet> descriptorSets);
        // Update single buffer
        void UpdateCommandBuffer(uint32_t imageIndex, std::vector<VkDescriptorSet> descriptorSets);
        // Begin single time commands
        VkCommandBuffer BeginSingleTimeCommands();
        // End single time commands
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

        // Cleanup
        void ClearCommandBuffers();
        void Cleanup();

        // Variables
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    private:
        // References
        Root* root;
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanRenderPass* vRenderPass;
        VulkanFrameBuffer* vFramebuffer;
    };
}