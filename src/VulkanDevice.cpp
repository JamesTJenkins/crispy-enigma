#include "VulkanDevice.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>

namespace VulkanModule {
    VulkanDevice::VulkanDevice(VulkanInstance* _vulkanInstance, VulkanSurface* _vulkanSurface) : vInstance(_vulkanInstance), vSurface(_vulkanSurface) {

    }

    VulkanDevice::~VulkanDevice() {

    }

    // Logical device creation

    void VulkanDevice::CreateLogicalDevice() {
        // Defines the number of queues per queue family
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		// Set queue priority
		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies){
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Specifing the required device features
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.sampleRateShading = VK_TRUE;

		// Creation of the logical device
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		// This is not required with newer versions of Vulkan but was suggested to be added anyway
		if (vInstance->enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(vInstance->validationLayers.size());
			createInfo.ppEnabledLayerNames = vInstance->validationLayers.data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create logical device.");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }

	void VulkanDevice::Cleanup() {
		vkDestroyDevice(device, nullptr);
	}

    // Helper functions

    void VulkanDevice::PickPhysicalDevice() {
		// Query all devices with vulkan support
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vInstance->instance, &deviceCount, nullptr);

		// If none found give up
		if (deviceCount == 0){
			throw std::runtime_error("Failed to find and graphic devices supporting Vulkan.");
		}

		// Create array to hold all possible devices
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vInstance->instance, &deviceCount, devices.data());

		// Use an ordered map to sort candidates by increasing score
		std::map<int, VkPhysicalDevice> candidates;

		// Process all devices
		for (const auto& device : devices) {
			// Check if even suitable
			if (IsDeviceSuitable(device)){
				// rate it
				int score = RateDevice(device);
				candidates.insert(std::make_pair(score, device));
			}
		}

		if (candidates.size() == 0){
			throw std::runtime_error("Failed to find a suitable device");
		}

		// Set to first in list
		std::pair<int, VkPhysicalDevice> curr = std::make_pair(candidates.begin()->first, candidates.begin()->second);
		
		std::map<int, VkPhysicalDevice>::iterator itr;
		for(itr = candidates.begin(); itr != candidates.end(); ++itr){
			if (itr->first > curr.first) {
				curr = std::make_pair(itr->first, itr->second);
			}
		}

		// Set physical device, and msaa
		physicalDevice = curr.second;
		msaaSamples = GetMaxUsableSampleCount();
	}

	int VulkanDevice::RateDevice(VkPhysicalDevice device){
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0;
		
		// Discrete GPUs have significant performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
			score += 1000;
		}

		// Maximum possible size of textures affect graphics quality
		score += deviceProperties.limits.maxImageDimension2D;

		// Application cant function without geometry shaders
		if (!deviceFeatures.geometryShader){
			return 0;
		}

		return score;
	}

	bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device){
		// Check queues
		QueueFamilyIndices indices = FindQueueFamilies(device);

		// Check extensions
		bool extensionSupported = CheckDeviceExtensionSupport(device);

		// Check swapchain support
		bool swapChainAdequate = false;
		if (extensionSupported) {
			SwapchainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.isComplete() && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		// Get the extension count
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		// Get the extensions
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		// Confirm the required extensions
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
		
		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device){
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vSurface->surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete())
				break;

			i++;
		}
		
		return indices;
	}

    VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount(){
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

		if (counts & VK_SAMPLE_COUNT_64_BIT)
			return VK_SAMPLE_COUNT_64_BIT;
		if (counts & VK_SAMPLE_COUNT_32_BIT)
			return VK_SAMPLE_COUNT_32_BIT;
		if (counts & VK_SAMPLE_COUNT_16_BIT)
			return VK_SAMPLE_COUNT_16_BIT;
		if (counts & VK_SAMPLE_COUNT_8_BIT)
			return VK_SAMPLE_COUNT_8_BIT;
		if (counts & VK_SAMPLE_COUNT_4_BIT)
			return VK_SAMPLE_COUNT_4_BIT;
		if (counts & VK_SAMPLE_COUNT_2_BIT)
			return VK_SAMPLE_COUNT_2_BIT;

		return VK_SAMPLE_COUNT_1_BIT;
	}

	SwapchainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device){
		SwapchainSupportDetails details;

		// Query basic surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vSurface->surface, &details.capabilities);

		// Query supported surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, vSurface->surface, &formatCount, nullptr);

		if (formatCount != 0){
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, vSurface->surface, &formatCount, details.formats.data());
		}

		// Query supported presentation modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vSurface->surface, &presentModeCount, nullptr);

		if (presentModeCount != 0){
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, vSurface->surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}
}