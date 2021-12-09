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
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(vSwapchain->swapchainImages.size());
        uniformBuffersMemory.resize(vSwapchain->swapchainImages.size());

        for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++) {
            vBuffer->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        }
    }

    void VulkanUniformBuffer::UpdateUniformBuffer(uint32_t currentImage) {
        // Modify ubo here
        UniformBufferObject ubo {};

        // Set to camera view settings
        ubo.view = root->activeScene.activeCamera->GetViewMatrix();
        // Set to camera projection setttings
        ubo.proj = root->activeScene.activeCamera->GetProjectionMatrix();
        // Set to cameras position
        ubo.viewPosition = root->activeScene.activeCamera->transformComponent->GetPosition();

        // Set lighting data
        ubo.ambient = glm::vec3(0.5f, 0.5f, 0.5f);

        // Get the lights
        std::vector<Components::LightData> l = GetLights();

        // Copy the lights in
        for (size_t i = 0; i < l.size(); ++i) {
            ubo.lights[i] = l[i];
        }

        // Set the amount of light
        ubo.numLights = l.size();

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

    std::vector<Components::LightData> VulkanUniformBuffer::GetLights() {
        std::vector<Components::LightData> importantLights;

        // Gets important lights

        // Get all directional lights
        for (size_t i = 0; i < root->lights[Components::DIRECTIONAL].size(); i++) {
            // Make sure to not go over the defined maximum
            if (i > MAX_NUM_LIGHTS - 1)
                return importantLights;

            Components::Light* light = root->lights[Components::DIRECTIONAL][i];

            Components::LightData data(glm::vec4(light->position, light->lightType), light->color, light->radius);
            importantLights.push_back(data);
        }
        
        // Process all point lights
        for (size_t i = 0; i < root->lights[Components::POINT].size(); i++) {
            // Make sure to not go over the defined maximum
            if (i > MAX_NUM_LIGHTS - 1)
                return importantLights;

            Components::Light* light = root->lights[Components::POINT][i];

            Components::LightData data(glm::vec4(light->position, light->lightType), light->color, light->radius);
            importantLights.push_back(data);
        }

        // Return
        return importantLights;
    }
}