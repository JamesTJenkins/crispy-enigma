#include "VulkanResources.h"

namespace VulkanModule {
    VulkanResources::VulkanResources(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanFrameBuffer* framebuffer, VulkanImageView* imageView, VulkanImage* image) : vDevice(device), vSwapchain(swapchain), vFramebuffer(framebuffer), vImageView(imageView), vImage(image) {

    }

    VulkanResources::~VulkanResources() {

    }

    void VulkanResources::CreateColorResources(){
		VkFormat colorFormat = vSwapchain->swapchainImageFormat;
        
		vImage->CreateImage(vSwapchain->swapchainExtent.width, vSwapchain->swapchainExtent.height, 1, vDevice->msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vFramebuffer->colorImage, vFramebuffer->colorImageMemory);
		vFramebuffer->colorImageView = vImageView->CreateImageView(vFramebuffer->colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void VulkanResources::CreateDepthResources(){
		VkFormat depthFormat = vImage->FindDepthFormat();

		vImage->CreateImage(vSwapchain->swapchainExtent.width, vSwapchain->swapchainExtent.height, 1, vDevice->msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vFramebuffer->depthImage, vFramebuffer->depthImageMemory);
		vFramebuffer->depthImageView = vImageView->CreateImageView(vFramebuffer->depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}
}