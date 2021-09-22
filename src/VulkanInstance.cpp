#include "VulkanInstance.h"
#include <SDL2/SDL_vulkan.h>
#include <iostream>

#include "Root.h"
#include "VK_EXT_debug_utils.h"

namespace VulkanModule {
    VulkanInstance::VulkanInstance(Root* _root) : root(_root) {

    }

    VulkanInstance::~VulkanInstance() {

    }

    // Instance

    void VulkanInstance::CreateInstance() {
        // Check validation layers
		if (enableValidationLayers && !CheckValidationLayerSupport()) {
			throw std::runtime_error("Validation layer requested, but not available.");
		}

		// Information about the game and its engine
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Test Game";
		appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
		appInfo.pEngineName = "Crispy Enigma";
		appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
        
        // Get SDL extensions
		std::vector<const char*> SDLExtensions = GetSDLExtensions(root->sdl2.window);

		// Tells vulkan driver which global extensions and validation layers will be used
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(SDLExtensions.size());
		createInfo.ppEnabledExtensionNames = SDLExtensions.data();
		
		// Enables validation layers
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		if (enableValidationLayers){
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			
			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		// Create instance
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
			throw std::runtime_error("Failed to create instance");
		} else {
			std::cout << "Created Vulkan instance successfully" << std::endl;
		}
    }

	void VulkanInstance::Cleanup() {
		if (enableValidationLayers)
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

		vkDestroyInstance(instance, nullptr);
	}

    // Helper functions

    std::vector<const char*> VulkanInstance::GetSDLExtensions(SDL_Window* window) {
        // Get SDL extensions for vulkan
		unsigned int count = 0;
		
		SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);

		std::vector<const char*> SDLExtensions = {
			// Additional extensions here
		};

		// Debugging extensions
		if (enableValidationLayers){
			SDLExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		size_t additionalExtensionCount = SDLExtensions.size();
		SDLExtensions.resize (additionalExtensionCount + count);

		SDL_Vulkan_GetInstanceExtensions(window, &count, SDLExtensions.data() + additionalExtensionCount);

		return SDLExtensions;
    }

    bool VulkanInstance::CheckValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
			
			if (!layerFound) {
				return false;
			}
		}
		
		return true;
	}

	// Debugging

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void VulkanInstance::SetupDebugMessenger(){
		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS){
			throw std::runtime_error("Failed to setup debug messenger.");
		}
	}

	void VulkanInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}
}