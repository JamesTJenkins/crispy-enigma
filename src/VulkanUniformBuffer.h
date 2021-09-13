#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanBuffer.h"

namespace VulkanModule {
    class VulkanUniformBuffer {
    public:
        // Constructors
        VulkanUniformBuffer(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanBuffer* buffer);
        ~VulkanUniformBuffer();

        // Create uniform buffers
        void CreateUniformBuffers();
        // Update a uniform buffer
        void UpdateUniformBuffer(uint32_t currentImage);

        // Cleanup
        void Cleanup();

        // Variables
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
    private:
        // References
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanBuffer* vBuffer;
    };
}