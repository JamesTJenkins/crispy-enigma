#include "VulkanBuffer.h"
#include <iostream>

namespace VulkanModule {
    VulkanBuffer::VulkanBuffer(VulkanDevice* device, VulkanCommand* commandBuffers) : vDevice(device), vCommand(commandBuffers) {

    }

    VulkanBuffer::~VulkanBuffer() {

    }

    void VulkanBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		// Sets the buffer size in bytes
		bufferInfo.size = size;
		// Specifies the usage of the buffer
		bufferInfo.usage = usage;
		// Sets the sharing mode so it will only be used by the graphics queue
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(vDevice->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS){
			throw std::runtime_error("Failed to create vertex buffer.");
		}

		// Setting memory requirements

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vDevice->device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(vDevice->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate vertex buffer memory.");
		}

		vkBindBufferMemory(vDevice->device, buffer, bufferMemory, 0);
	}

	void VulkanBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size){
		// Create command buffer
		VkCommandBuffer commandBuffer = vCommand->BeginSingleTimeCommands();

		// Tell driver we are only using the buffer until the copy is complete

		VkBufferCopy copyRegion{};
		copyRegion.size = size;

		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	
		// Destroy command buffer
		vCommand->EndSingleTimeCommands(commandBuffer);
	}

	uint32_t VulkanBuffer::FindMemoryType(uint32_t typeFiler, VkMemoryPropertyFlags properties){
		VkPhysicalDeviceMemoryProperties memProperties;
		// FIX (cant get physical device)
        vkGetPhysicalDeviceMemoryProperties(vDevice->physicalDevice, &memProperties);
        
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFiler & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
		
		throw std::runtime_error("Failed to find suitable memory type.");
	}

	void VulkanBuffer::CreateVertexBuffer(Data::Mesh* mesh){
		// Create Buffer

		VkDeviceSize bufferSize = sizeof(mesh->vertices[0]) * mesh->vertices.size();
		
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Filling the vertex buffer

		void* data;
		vkMapMemory(vDevice->device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, mesh->vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(vDevice->device, stagingBufferMemory);

		// Staging buffer to vertex buffer
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mesh->vertexBuffer, mesh->vertexBufferMemory);
	
		CopyBuffer(stagingBuffer, mesh->vertexBuffer, bufferSize);

		// Clear the staging buffer
		vkDestroyBuffer(vDevice->device, stagingBuffer, nullptr);
		vkFreeMemory(vDevice->device, stagingBufferMemory, nullptr);
	}

	void VulkanBuffer::CreateIndexBuffer(Data::Mesh* mesh){
		VkDeviceSize bufferSize = sizeof(mesh->indices[0]) * mesh->indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Filling the vertex buffer

		void* data;
		vkMapMemory(vDevice->device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, mesh->indices.data(), (size_t)bufferSize);
		vkUnmapMemory(vDevice->device, stagingBufferMemory);

		// Staging buffer to vertex buffer
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mesh->indexBuffer, mesh->indexBufferMemory);
	
		CopyBuffer(stagingBuffer, mesh->indexBuffer, bufferSize);

		// Clear the staging buffer
		vkDestroyBuffer(vDevice->device, stagingBuffer, nullptr);
		vkFreeMemory(vDevice->device, stagingBufferMemory, nullptr);
	}
} 
