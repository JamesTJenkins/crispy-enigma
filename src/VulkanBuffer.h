#pragma once
#include <vulkan/vulkan.h>

#include "Mesh.h"

#include "VulkanDevice.h"
#include "VulkanCommand.h"

namespace VulkanModule {
    class VulkanBuffer {
    public:
        // Constructor
        VulkanBuffer(VulkanDevice* device, VulkanCommand* commandBuffers);
        ~VulkanBuffer();

        // Create buffer
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        // Copy buffer
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        // Get memory type
        uint32_t FindMemoryType(uint32_t typeFiler, VkMemoryPropertyFlags properties);
        // Create vertex buffer
        void CreateVertexBuffer(Data::Mesh* mesh);
        // Create index buffer
        void CreateIndexBuffer(Data::Mesh* mesh);
    private:
        // References
        VulkanDevice* vDevice;
        VulkanCommand* vCommand;
    };
}