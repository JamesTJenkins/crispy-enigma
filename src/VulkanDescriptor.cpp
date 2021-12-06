#include "VulkanDescriptor.h"
#include <iostream>

#include "Root.h"

/*

        This needs some cleanup at somepoint to remove redundant code

*/

namespace VulkanModule {
    VulkanDescriptor::VulkanDescriptor(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanUniformBuffer* uniformbuffer) : root(_root), vDevice(device), vSwapchain(swapchain), vUniformbuffer(uniformbuffer) {

    }

    VulkanDescriptor::~VulkanDescriptor() {

    }

    void VulkanDescriptor::CreateEmptyDescriptorSetLayout() {
        // Setup binding

        // UBO
        VkDescriptorSetLayoutBinding uboLayoutBinding {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // Used for image sampling descriptors
        uboLayoutBinding.pImmutableSamplers = nullptr;
        // Which shader stage the descriptor gets referenced
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        // Sampler
        VkDescriptorSetLayoutBinding samplerLayoutBinding {};
        samplerLayoutBinding.binding = 1;
        // Set to the amount of textures loaded
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

        // Setup layout
        VkDescriptorSetLayoutCreateInfo layoutInfo {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(vDevice->device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout.");
        }
    }

    void VulkanDescriptor::CreateDescriptorSetLayout() {
        // Cleanup previous layout
        Cleanup();

        // Setup binding

        // UBO
        VkDescriptorSetLayoutBinding uboLayoutBinding {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // Used for image sampling descriptors
        uboLayoutBinding.pImmutableSamplers = nullptr;
        // Which shader stage the descriptor gets referenced
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        // Sampler
        VkDescriptorSetLayoutBinding samplerLayoutBinding {};
        samplerLayoutBinding.binding = 1;
        // Set to the amount of textures loaded
        samplerLayoutBinding.descriptorCount = root->assetManager.loadedTextures.size();
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

        // Setup layout
        VkDescriptorSetLayoutCreateInfo layoutInfo {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(vDevice->device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout.");
        }
    }

    void VulkanDescriptor::CreateEmptyDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(vSwapchain->swapchainImages.size());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(vSwapchain->swapchainImages.size());

        VkDescriptorPoolCreateInfo poolInfo {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(vSwapchain->swapchainImages.size());

        if (vkCreateDescriptorPool(vDevice->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool.");
        }
    }

    void VulkanDescriptor::CreateDescriptorPool() {
        // Create the pool
        std::array<VkDescriptorPoolSize, 2> poolSizes {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(vSwapchain->swapchainImages.size());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(root->assetManager.loadedTextures.size() * vSwapchain->swapchainImages.size());

        VkDescriptorPoolCreateInfo poolInfo {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(vSwapchain->swapchainImages.size());

        if (vkCreateDescriptorPool(vDevice->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool.");
        }
    }

    void VulkanDescriptor::CreateDescriptorSets() {
        // Will create a set for every swapchain image
        std::vector<VkDescriptorSetLayout> layouts(vSwapchain->swapchainImages.size(), descriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(vSwapchain->swapchainImages.size());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(vSwapchain->swapchainImages.size());
        if (vkAllocateDescriptorSets(vDevice->device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate descriptor sets.");
        }

        // Populate all the descriptor sets
        for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++) {
            // Where all descriptor sets are held
            std::array<VkWriteDescriptorSet, 2> descriptorWrites {};

            // Set UBO binding
            VkDescriptorBufferInfo bufferInfo {};
            bufferInfo.buffer = vUniformbuffer->uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(Data::UniformBufferObject);

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            // Set texture and sampler bindings
            std::vector<VkDescriptorImageInfo> imageInfos;
            for (auto& t : root->assetManager.loadedTextures) {
                VkDescriptorImageInfo imageInfo;
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = t.second.textureImageView;
                imageInfo.sampler = t.second.textureSampler;
                imageInfos.insert(imageInfos.begin(), imageInfo);
            }

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = root->assetManager.loadedTextures.size();
            descriptorWrites[1].pBufferInfo = 0;
            descriptorWrites[1].pImageInfo = imageInfos.data();

            // Update descriptor set
            vkUpdateDescriptorSets(vDevice->device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    void VulkanDescriptor::ClearPool() {
        if (descriptorPool != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(vDevice->device, descriptorPool, nullptr);
    }

    void VulkanDescriptor::Cleanup() {
        vkDestroyDescriptorSetLayout(vDevice->device, descriptorSetLayout, nullptr);
    }
}