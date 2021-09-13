#include "VulkanImageView.h"
#include <iostream>

namespace VulkanModule {
    VulkanImageView::VulkanImageView(VulkanDevice* device, VulkanSwapchain* swapchain) : vDevice(device), vSwapchain(swapchain) {

    }

    VulkanImageView::~VulkanImageView() {

    }

    void VulkanImageView::CreateImageViews(){
		swapchainImageViews.resize(vSwapchain->swapchainImages.size());

		for (size_t i = 0; i < vSwapchain->swapchainImages.size(); i++){
			swapchainImageViews[i] = CreateImageView(vSwapchain->swapchainImages[i], vSwapchain->swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

    VkImageView VulkanImageView::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels){
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(vDevice->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS){
			throw std::runtime_error("Failed to create texture image view.");
		}

		return imageView;
	}

	void VulkanImageView::Cleanup() {
		for (size_t i = 0; i < swapchainImageViews.size(); i++){
			vkDestroyImageView(vDevice->device, swapchainImageViews[i], nullptr);
		}
	}

	VkFormat VulkanImageView::FindDepthFormat(){
		return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	VkFormat VulkanImageView::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features){
		for (VkFormat format : candidates){
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(vDevice->physicalDevice, format, &props);
		
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features){
				return format;
			} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		
		throw std::runtime_error("Failed to find supported format.");
	}
}