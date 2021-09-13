#pragma once
#include <vulkan/vulkan.h>
#include <optional>

#include "VulkanInstance.h"
#include "VulkanSurface.h"

namespace VulkanModule {
    struct QueueFamilyIndices {
		// If it holds a value it means the device is supported
		std::optional<uint32_t> graphicsFamily;
        // For window support checking
        std::optional<uint32_t> presentFamily;

		bool isComplete(){
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

    // Settings for the swapchain
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanDevice {
    public:
        VulkanDevice(VulkanInstance* _vulkanInstance, VulkanSurface* _vulkanSurface);
        ~VulkanDevice();

        // Creates a logical device
        void CreateLogicalDevice();

        // Cleanup
        void Cleanup();

        // Helper functions

        void PickPhysicalDevice();
        int RateDevice(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        VkSampleCountFlagBits GetMaxUsableSampleCount();
        SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

        // Variables

        VkDevice device;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    private:
        VulkanInstance* vInstance;
        VulkanSurface* vSurface;

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };
}