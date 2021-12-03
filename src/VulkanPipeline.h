#pragma once
#include <vulkan/vulkan.h>
#include <iostream>

#include "Root.fwd.h"
#include "Materials.h"

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptor.h"

namespace VulkanModule {
    struct PushConstants {
        glm::mat4 model;
        int texId;
    };

    class VulkanPipeline {
    public:
        // Constructor
        VulkanPipeline(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanRenderPass* renderpass, VulkanDescriptor* descriptor);
        ~VulkanPipeline();

        // Create graphics pipeline
        void CreateGraphicsPipeline(std::string shaderName);
        // Create graphics pipeline variant
        void CreateGraphicsPipeline(Data::Shader* shader);
        // Create shader module
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        // Create all loaded graphics pipelines
        void CreateAllGraphicPipelines();

        // Cleanup
        void Cleanup();
    private:
        // Reference
        Root* root;
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        VulkanRenderPass* vRenderpass;
        VulkanDescriptor* vDescriptor;
    };
}