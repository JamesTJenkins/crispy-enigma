#include "VulkanUniformBuffer.h"

#include "Mesh.h"

namespace VulkanModule {
    VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanBuffer* buffer) : vDevice(device), vSwapchain(swapchain), vBuffer(buffer) {

    }

    VulkanUniformBuffer::~VulkanUniformBuffer() {

    }

    void VulkanUniformBuffer::CreateUniformBuffers(){
		VkDeviceSize bufferSize = sizeof(Data::UniformBufferObject);

		uniformBuffers.resize(vSwapchain->swapchainImages.size());
		uniformBuffersMemory.resize(vSwapchain->swapchainImages.size());
		
		for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++){
			vBuffer->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
		}
	}

	void VulkanUniformBuffer::UpdateUniformBuffer(uint32_t currentImage){
		// Modify ubo here
		Data::UniformBufferObject ubo{};

		// Copy data into buffer
		void* data;
		vkMapMemory(vDevice->device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(vDevice->device, uniformBuffersMemory[currentImage]);
	}

	void VulkanUniformBuffer::Cleanup() {
		for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++){
			vkDestroyBuffer(vDevice->device, uniformBuffers[i], nullptr);
			vkFreeMemory(vDevice->device, uniformBuffersMemory[i], nullptr);
		}
	}
}