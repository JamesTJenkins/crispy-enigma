#include "Vulkan.h"
#include <iostream>

#include "Root.h"

namespace VulkanModule {
    Vulkan::Vulkan(Root* _root) : root(_root), vWindow(_root), vInstance(_root), vSurface(root, &vInstance), vDevice(&vInstance, &vSurface), vSwapchain(root, &vDevice, &vSurface), vImageView(&vDevice, &vSwapchain), vRenderPass(&vDevice, &vSwapchain, &vImageView), vDescriptor(_root, &vDevice, &vSwapchain, &vUniformBuffer), vFramebuffer(_root, &vDevice, &vSwapchain, &vImageView, &vRenderPass), vCommandbuffer(_root, &vDevice, &vSwapchain, &vRenderPass, &vFramebuffer), vBuffer(&vDevice, &vCommandbuffer), vImage(&vDevice, &vCommandbuffer, &vBuffer), vResources(&vDevice, &vSwapchain, &vFramebuffer, &vImageView, &vImage), vUniformBuffer(_root, &vDevice, &vSwapchain, &vBuffer), vSync(&vDevice, &vSwapchain, &MAX_FRAMES_IN_FLIGHT), vPipeline(_root, &vDevice, &vSwapchain, &vRenderPass, &vDescriptor) {
        // Get extension count
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		// Get extensions
		extensions = std::vector<VkExtensionProperties>(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		// Print off all available extensions
		std::cout << "Available extensions: " << extensionCount << std::endl;
    }

    Vulkan::~Vulkan() {
        
    }

    void Vulkan::InitVulkan() {
        vWindow.CreateVulkanWindow("Test");
        vInstance.CreateInstance();
        vInstance.SetupDebugMessenger();
        vSurface.CreateSurface();
        vDevice.PickPhysicalDevice();
        vDevice.CreateLogicalDevice();
        vSwapchain.CreateSwapchain();
        vImageView.CreateImageViews();
        vRenderPass.CreateRenderPass();
        vDescriptor.CreateEmptyDescriptorSetLayout();
        vPipeline.CreateAllGraphicPipelines();
        vCommandbuffer.CreateCommandPool();
        vResources.CreateColorResources();
        vResources.CreateDepthResources();
        vFramebuffer.CreateFramebuffers();
        vUniformBuffer.CreateUniformBuffers();
        vDescriptor.CreateEmptyDescriptorPool();
        //vDescriptor.CreateDescriptorSets(); (This gets generated later)
        vCommandbuffer.CreateCommandBuffers(vDescriptor.descriptorSets);
        vSync.CreateSyncObjects();

        std::cout << "Vulkan Initialized" << std::endl;
    }

    void Vulkan::DrawFrame() {
        // Waits for the frame to be finished
		vkWaitForFences(vDevice.device, 1, &vSync.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		// Get the image from the swapchain
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(vDevice.device, vSwapchain.swapchain, UINT64_MAX, vSync.imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

        // Update command buffers per frame (probs just make it that a single buffer gets updated per frame rather than all)
        vCommandbuffer.UpdateCommandBuffer(imageIndex, vDescriptor.descriptorSets);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			WindowChanged();
			return;
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to aquire swapchain image.");
		}

		// Check if previous frame is still using this image
		if (vSync.imagesInFlight[imageIndex] != VK_NULL_HANDLE){
			vkWaitForFences(vDevice.device, 1, &vSync.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}

		// Mark the image as now being used by this frame
		vSync.imagesInFlight[imageIndex] = vSync.inFlightFences[currentFrame];

		// Update uniform buffer
		vUniformBuffer.UpdateUniformBuffer(imageIndex);

		// Submit the command buffer and sync
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { vSync.imageAvailableSemaphore[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// Which commands to be submitted
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vCommandbuffer.commandBuffers[imageIndex];

		// Sets semaphores to signal when the command buffer is finished
		VkSemaphore signalSemaphores[] = { vSync.renderFinishedSemaphore[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(vDevice.device, 1, &vSync.inFlightFences[currentFrame]);

		// Submit
		if (vkQueueSubmit(vDevice.graphicsQueue, 1, &submitInfo, vSync.inFlightFences[currentFrame]) != VK_SUCCESS){
			throw std::runtime_error("Failed to submit draw command buffer.");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapchains[] = { vSwapchain.swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imageIndex;
		// This can be used to check multiple swapchains for a result
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR(vDevice.presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vFramebuffer.framebufferResized){
			vFramebuffer.framebufferResized = false;
			WindowChanged();
		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swapchain image");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Vulkan::SetWindowSize(int width, int height) {
        // Set width and height variables
		root->sdl2.width = width;
		root->sdl2.height = height;
		// Resize the window and flag for swapchain to be updated
		SDL_SetWindowSize(root->sdl2.window, width, height);
		vFramebuffer.framebufferResized = true;
    }

    void Vulkan::WindowChanged() {
        // For minimizing
		int flags = SDL_GetWindowFlags(root->sdl2.window);
		bool isMinimized = (flags & SDL_WINDOW_MINIMIZED) ? true : false;

		while (isMinimized){
			int flags = SDL_GetWindowFlags(root->sdl2.window);
			isMinimized = (flags & SDL_WINDOW_MINIMIZED) ? true : false;

			SDL_WaitEvent(nullptr);
		}

		CleanupOldSwapchain();
		BuildSwapchain();
    }

    void Vulkan::BuildSwapchain() {
        vSwapchain.CreateSwapchain();
        vImageView.CreateImageViews();
        vRenderPass.CreateRenderPass();
        vDescriptor.CreateDescriptorSetLayout();
        vPipeline.CreateAllGraphicPipelines();
        vResources.CreateColorResources();
        vResources.CreateDepthResources();
        vFramebuffer.CreateFramebuffers();
        vUniformBuffer.CreateUniformBuffers();
        vDescriptor.CreateDescriptorPool();
        vDescriptor.CreateDescriptorSets();
        vCommandbuffer.CreateCommandBuffers(vDescriptor.descriptorSets);

        vSync.imagesInFlight.resize(vSwapchain.swapchainImages.size(), VK_NULL_HANDLE);
    }

    void Vulkan::CleanupOldSwapchain() {
        // Wait for logical device to finish operations
		vkDeviceWaitIdle(vDevice.device);

        vFramebuffer.Cleanup();
        vCommandbuffer.ClearCommandBuffers();
        vPipeline.Cleanup();
        vRenderPass.Cleanup();
		vImageView.Cleanup();
        vSwapchain.Cleanup();
        vUniformBuffer.Cleanup();
		vDescriptor.ClearPool();
    }

    void Vulkan::Cleanup() {
        // Wait for logical device to finish operations
		vkDeviceWaitIdle(vDevice.device);
        
		// Cleanup
		CleanupOldSwapchain();
    
        CleanupOldTextures();
        CleanupOldMeshes();
        
        vDescriptor.Cleanup();
        vSync.Cleanup();
        vCommandbuffer.Cleanup();
        vDevice.Cleanup();
        vSurface.Cleanup();
        vInstance.Cleanup();

        std::cout << "Vulkan cleaned up" << std::endl;
    }

    void Vulkan::CleanupOldTextures() {
        for (const auto& vt : root->assetManager.loadedTextures) {
			vkDestroySampler(vDevice.device, vt.second.textureSampler, nullptr);
			vkDestroyImageView(vDevice.device, vt.second.textureImageView, nullptr);
			vkDestroyImage(vDevice.device, vt.second.textureImage, nullptr);
			vkFreeMemory(vDevice.device, vt.second.textureImageMemory, nullptr);
		}
    }

    void Vulkan::CleanupOldMeshes() {
		for (const auto& v : root->assetManager.loadedMeshes) {
			vkDestroyBuffer(vDevice.device, v.second.indexBuffer, nullptr);
			vkFreeMemory(vDevice.device, v.second.indexBufferMemory, nullptr);
			vkDestroyBuffer(vDevice.device, v.second.vertexBuffer, nullptr);
			vkFreeMemory(vDevice.device, v.second.vertexBufferMemory, nullptr);
		}
    }
}