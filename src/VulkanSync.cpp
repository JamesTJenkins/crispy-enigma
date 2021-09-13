#include "VulkanSync.h"
#include <iostream>

namespace VulkanModule {
    VulkanSync::VulkanSync(VulkanDevice* device, VulkanSwapchain* swapchain, const size_t* maxFramesInFlight) : vDevice(device), vSwapchain(swapchain), MAX_FRAMES_IN_FLIGHT(maxFramesInFlight) {

    }

    VulkanSync::~VulkanSync() {

    }

    void VulkanSync::CreateSyncObjects(){
		imageAvailableSemaphore.resize(*MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphore.resize(*MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(*MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(vSwapchain->swapchainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < *MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(vDevice->device, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != VK_SUCCESS || vkCreateSemaphore(vDevice->device, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != VK_SUCCESS || vkCreateFence(vDevice->device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS){
				throw std::runtime_error("Failed to create synchronization objects for a frame.");
			}
		}
	}

	void VulkanSync::Cleanup() {
		for (size_t i = 0; i < *MAX_FRAMES_IN_FLIGHT; i++){
			vkDestroySemaphore(vDevice->device, renderFinishedSemaphore[i], nullptr);
			vkDestroySemaphore(vDevice->device, imageAvailableSemaphore[i], nullptr);
			vkDestroyFence(vDevice->device, inFlightFences[i], nullptr);
		}
	}
}