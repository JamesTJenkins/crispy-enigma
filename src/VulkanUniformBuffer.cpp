#include "VulkanUniformBuffer.h"
#include <assert.h>

#include "Root.h"
#include "Mesh.h"

namespace VulkanModule {
    VulkanUniformBuffer::VulkanUniformBuffer(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanBuffer* buffer) : root(_root), vDevice(device), vSwapchain(swapchain), vBuffer(buffer) {

    }

    VulkanUniformBuffer::~VulkanUniformBuffer() {

    }

    void VulkanUniformBuffer::CreateUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(Data::UniformBufferObject);

        uniformBuffers.resize(vSwapchain->swapchainImages.size());
        uniformBuffersMemory.resize(vSwapchain->swapchainImages.size());

        for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++) {
            vBuffer->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        }
    }

    void VulkanUniformBuffer::UpdateUniformBuffer(uint32_t currentImage) {
        // Modify ubo here
        Data::UniformBufferObject ubo {};
        
        // Set to camera view settings
        ubo.view = root->activeScene.activeCamera->GetViewMatrix();
        // Set to camera projection setttings
        ubo.proj = root->activeScene.activeCamera->GetProjectionMatrix();
        
        // Flip Y coord as its wrong in vulkan
        //ubo.proj[1][1] *= -1;

        // Copy data into buffer
        void* data;
        vkMapMemory(vDevice->device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(vDevice->device, uniformBuffersMemory[currentImage]);
    }

    void VulkanUniformBuffer::Cleanup() {
        for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++) {
            vkDestroyBuffer(vDevice->device, uniformBuffers[i], nullptr);
            vkFreeMemory(vDevice->device, uniformBuffersMemory[i], nullptr);
        }
    }
}