#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanUniformBuffer.h"

namespace VulkanModule {
    class VulkanDescriptor {
    public:
        // Constructors
        VulkanDescriptor(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanUniformBuffer* uniformbuffer);
        ~VulkanDescriptor();

        // Creates the descriptor set layout
        void CreateDescriptorSetLayout();
        // Creates the descriptor pool
        void CreateDescriptorPool();
        // Creates descriptor sets
        void CreateDescriptorSets();

        // Clear descriptor pool
        void ClearPool();
        // Cleanup
        void Cleanup();

        // Variables
        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<VkDescriptorSet> descriptorSets;
    private:
        // References
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanUniformBuffer* vUniformbuffer;
    };
}