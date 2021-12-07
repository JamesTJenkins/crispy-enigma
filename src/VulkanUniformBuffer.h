#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Root.fwd.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanBuffer.h"
#include "Light.h"

#define MAX_NUM_LIGHTS 8

namespace VulkanModule {

    struct UniformBufferObject {
        // Camera
        glm::mat4 view;
        glm::mat4 proj;
        alignas(16) glm::vec3 viewPosition;
        // Lights
        alignas(16) glm::vec3 ambient;
        int numLights;
        Components::LightData lights[MAX_NUM_LIGHTS];
    };

    struct DynamicBufferObject {
        glm::mat4* model;
    };

    class VulkanUniformBuffer {
    public:
        // Constructors
        VulkanUniformBuffer(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanBuffer* buffer);
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
        std::vector<Components::LightData> GetLights();

        // References
        Root* root;
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanBuffer* vBuffer;
    };
}