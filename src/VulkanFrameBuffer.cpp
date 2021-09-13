#include "VulkanFrameBuffer.h"
#include <iostream>
#include <array>

#include "Root.h"

namespace VulkanModule {
    VulkanFrameBuffer::VulkanFrameBuffer(Root* _root, VulkanDevice* device, VulkanSwapchain* swapchain, VulkanImageView* imageView, VulkanRenderPass* renderpass) : root(_root), vDevice(device), vSwapchain(swapchain), vImageView(imageView), vRenderpass(renderpass) {

    }

    VulkanFrameBuffer::~VulkanFrameBuffer() {

    }

    void VulkanFrameBuffer::CreateFramebuffers(){
		swapchainFramebuffers.resize(vImageView->swapchainImageViews.size());

		// Iterates through all image views and creates framebuffers for each of them
		for (size_t i = 0; i < vImageView->swapchainImageViews.size(); i++) {
			std::array<VkImageView, 3> attachments = {
				colorImageView,
				depthImageView,
				vImageView->swapchainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = vRenderpass->renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = vSwapchain->swapchainExtent.width;
			framebufferInfo.height = vSwapchain->swapchainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(vDevice->device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS){
				throw std::runtime_error("Failed to create framebuffer.");
			}
		}
	}

	void VulkanFrameBuffer::FramebufferResized(){
		// Change window size
		SDL_SetWindowSize(root->sdl2.window, root->sdl2.width, root->sdl2.height);
		// Flag for swapchain to be updated
		framebufferResized = true;
	}

	void VulkanFrameBuffer::Cleanup() {
		vkDestroyImageView(vDevice->device, colorImageView, nullptr);
		vkDestroyImage(vDevice->device, colorImage, nullptr);
		vkFreeMemory(vDevice->device, colorImageMemory, nullptr);

		vkDestroyImageView(vDevice->device, depthImageView, nullptr);
		vkDestroyImage(vDevice->device, depthImage, nullptr);
		vkFreeMemory(vDevice->device, depthImageMemory, nullptr);

		for (size_t i = 0; i < swapchainFramebuffers.size(); i++){
			vkDestroyFramebuffer(vDevice->device, swapchainFramebuffers[i], nullptr);
		}
	}
}