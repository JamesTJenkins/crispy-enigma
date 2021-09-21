#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Root.fwd.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanUniformBuffer.h"

namespace VulkanModule {
    class VulkanDescriptor {
    public:
        // Constructors
        VulkanDescriptor(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanUniformBuffer* uniformbuffer);
        ~VulkanDescriptor();

        // Create empty descriptor set layout
        void CreateEmptyDescriptorSetLayout();
        // Creates the descriptor set layout
        void CreateDescriptorSetLayout();
        // Create empty descriptor pool
        void CreateEmptyDescriptorPool();
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
        uint32_t GetRenderDataSize();

        // References
        Root* root;
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanUniformBuffer* vUniformbuffer;
    };
}