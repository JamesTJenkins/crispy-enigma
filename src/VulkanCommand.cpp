#include "VulkanCommand.h"
#include <iostream>
#include <array>

namespace VulkanModule {
    VulkanCommand::VulkanCommand(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanRenderPass* renderpass, VulkanFrameBuffer* framebuffer) : vDevice(device), vSwapchain(swapchain), vRenderPass(renderpass), vFramebuffer(framebuffer) {

    }

    VulkanCommand::~VulkanCommand() {

    }

    void VulkanCommand::CreateCommandPool() {
		QueueFamilyIndices queueFamilyIndices = vDevice->FindQueueFamilies(vDevice->physicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(vDevice->device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create command pool.");
		}
	}

	void VulkanCommand::CreateCommandBuffers() {
		commandBuffers.resize(vFramebuffer->swapchainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(vDevice->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command buffers.");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++){
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("Failed to begin recording command buffer.");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vRenderPass->renderPass;
			renderPassInfo.framebuffer = vFramebuffer->swapchainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = vSwapchain->swapchainExtent;
	
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
			clearValues[1].depthStencil = {1.0f, 0};

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();
	
			// Will only use primary command buffer
			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            /*
			for (auto& objects : bufferObjects) {
				// Bind this to the graphics pipeline
				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, assetManager->loadedShaders[objects.first].graphicsPipeline);
			
				for (auto& obj : objects.second) {
					// Bind vertex and index buffer
					VkBuffer vertexBuffers[] = { assetManager->loadedMeshes[obj.meshRenderer->meshRef].vertexBuffer };
					VkDeviceSize offsets[] = { 0 };
			
					vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
					vkCmdBindIndexBuffer(commandBuffers[i], assetManager->loadedMeshes[obj.meshRenderer->meshRef].indexBuffer, 0, VK_INDEX_TYPE_UINT32);
					vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, assetManager->loadedShaders[objects.first].pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
					// Draw
					vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(assetManager->loadedMeshes[obj.meshRenderer->meshRef].indices.size()), 1, 0, 0, 0);
				}
			}
            */

			// End renderpass
			vkCmdEndRenderPass(commandBuffers[i]);

			// End the command buffer
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
				throw std::runtime_error("Failed to record command buffer");
			}
		}
	}

    VkCommandBuffer VulkanCommand::BeginSingleTimeCommands(){
		// Setup the command buffer
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vDevice->device, &allocInfo, &commandBuffer);

		// Begin recording of command buffer
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanCommand::EndSingleTimeCommands(VkCommandBuffer commandBuffer){
		// Stop recording of command buffer
		vkEndCommandBuffer(commandBuffer);

		// Submit commands
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vDevice->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vDevice->graphicsQueue);

		// Clear command buffer
		vkFreeCommandBuffers(vDevice->device, commandPool, 1, &commandBuffer);
	}

	void VulkanCommand::ClearCommandBuffers() {
		vkFreeCommandBuffers(vDevice->device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	}

	void VulkanCommand::Cleanup() {
		vkDestroyCommandPool(vDevice->device, commandPool, nullptr);
	}
}