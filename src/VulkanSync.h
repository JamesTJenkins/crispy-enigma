#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

namespace VulkanModule {
    class VulkanSync {
    public:
        // Constructor
        VulkanSync(VulkanDevice* device, VulkanSwapchain* swapchain, const size_t* maxFramesInFlight);
        ~VulkanSync();

        // Create sync objects
        void CreateSyncObjects();

        // Cleanup
        void Cleanup();

        // Variables
        std::vector<VkSemaphore> imageAvailableSemaphore;
        std::vector<VkSemaphore> renderFinishedSemaphore;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
    private:
        // References
        VulkanDevice* vDevice;
        VulkanSwapchain* vSwapchain;
        const size_t* MAX_FRAMES_IN_FLIGHT;
    };
}