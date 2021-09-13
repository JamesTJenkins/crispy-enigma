#include "VulkanSwapchain.h"
#include <glm.hpp>
#include <iostream>

#include "Root.h"

namespace VulkanModule {
    VulkanSwapchain::VulkanSwapchain(Root* _root, VulkanDevice* _vulkanDevice, VulkanSurface* _vulkanSurface) : root(_root), vDevice(_vulkanDevice), vSurface(_vulkanSurface) {

    }

    VulkanSwapchain::~VulkanSwapchain() {

    }

    void VulkanSwapchain::CreateSwapchain(){
		SwapchainSupportDetails swapChainSupport = vDevice->QuerySwapChainSupport(vDevice->physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, root->sdl2.width, root->sdl2.height);
	
		// Get the minimum needed to function then add one to avoid waiting on the driver to finish internal operations
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	
		// Stops going over the maximum limit
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		// Create swapchain object
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = vSurface->surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		// This is the amount of layers per image, only needed to be more than 1 for steroscopic 3d apps
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = vDevice->FindQueueFamilies(vDevice->physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		// This setup is to avoid ownership setup
		// Settings to handle swapchain images over multiple queue families
		if (indices.graphicsFamily != indices.presentFamily) {
			// If queue families differ
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			// If queue families are the same
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			// Optional
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		// Allows certain transforms to the image i.e. flipping is supported
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		// Specifies if the alpha channel should be used for blending windows
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		// Defines that we dont care about the color of obscured pixels
		createInfo.clipped = VK_TRUE;
		// Used when the swapchain becomes invalid or unoptimal, i.e. window resize
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		// Create the swapchain
		if (vkCreateSwapchainKHR(vDevice->device, &createInfo, nullptr, &swapchain) != VK_SUCCESS){
			throw std::runtime_error("Failed to create swapchain.");
		}

		vkGetSwapchainImagesKHR(vDevice->device, swapchain, &imageCount, nullptr);
		swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(vDevice->device, swapchain, &imageCount, swapchainImages.data());
	
		swapchainImageFormat = surfaceFormat.format;
		swapchainExtent = extent;
	}

	void VulkanSwapchain::Cleanup() {
		vkDestroySwapchainKHR(vDevice->device, swapchain, nullptr);
	}

	VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
		// Attempts to get ideal format for colorspace
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
				return availableFormat;
			}
		}
		
		// If fails just get the first
		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){
		// How the swapchain functions, mailbox is ideal
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR){
				return availablePresentMode;
			}
		}
		
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int windowWidth, int windowHeight){
		// Resolution of the swapchain images, and will be the resolution of the window in pixels
		
		// Default size is the window size
		VkExtent2D size = { (unsigned int)windowWidth, (unsigned int)windowHeight };
	
		// If the window scales based on the size of the image
		if (capabilities.currentExtent.width == 0xFFFFFFF) {
			size.width = glm::clamp<unsigned int>(size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			size.height = glm::clamp<unsigned int>(size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		} else {
			size = capabilities.currentExtent;
		}

		return size;
	}
}