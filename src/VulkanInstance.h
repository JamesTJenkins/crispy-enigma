#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Root.fwd.h"
#include "SDL2.h"

namespace VulkanModule {
    class VulkanInstance {
    public:
        VulkanInstance(Root* _root);
        ~VulkanInstance();

        // Creates a vulkan instance with required validation layers
        void CreateInstance();

        // Cleanup
        void Cleanup();

        // Helper functions

        std::vector<const char*> GetSDLExtensions(SDL_Window* window);
        bool CheckValidationLayerSupport();

        // Debugging callback

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        void SetupDebugMessenger();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        // Variables

        VkInstance instance = NULL;

        // DEBUGGING
        VkDebugUtilsMessengerEXT debugMessenger;

        // Set needed validation layers here
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    private:
        Root* root;
    };
}