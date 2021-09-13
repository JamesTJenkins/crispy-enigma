#pragma once
#include <vulkan/vulkan.h>

#include "Materials.h"

#include "VulkanDevice.h"
#include "VulkanCommand.h"
#include "VulkanBuffer.h"

namespace VulkanModule {
    class VulkanImage {
    public:
        // Constructor
        VulkanImage(VulkanDevice* device, VulkanCommand* commandBuffer, VulkanBuffer* buffer);
        ~VulkanImage();

        // Create image texture
        void CreateTextureImage(Data::Texture* texture);
        // Create an image
        void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        // Changing image layout
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
        // Copying image
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        // Create texture image view
        void CreateTextureImageView(Data::Texture* texture);
        // Create an image view
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        // Create texture sampler
        void CreateTextureSampler(Data::Texture* texture);
        // Generate mipmaps
        void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    
        // Helper functions
        VkFormat FindDepthFormat();
        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    private:
        // References
        VulkanDevice* vDevice;
        VulkanCommand* vCommand;
        VulkanBuffer* vBuffer;
    };
}