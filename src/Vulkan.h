#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <unordered_map>
#include "SDL2.h"
#include "AssetManager.h"

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

    struct VMesh {
        Data::Mesh* meshRef;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
    };

    struct VShader {
        uint32_t id;
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
    };

    struct VTexture {
        Data::Texture* textureRef;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkSampler textureSampler;
        VkImageView textureImageView;
    };

    struct Renderable {
        VMesh* mesh;
        VShader* shader;
    };

    class Vulkan {
    public:
        Vulkan();
        ~Vulkan();

        void InitVulkan(SDLModule::SDL2* _sdl2, Manager::AssetManager* _assetManager);
        void DrawFrame();

        void SetWindowSize(int width, int height);
        
        void FramebufferResized();
        void RebuildSwapchain();
        
        void CleanupOldSwapchain();
        void Cleanup();

        void CreateVulkanWindow(const char *title);

        void CreateInstance();

        void PickPhysicalDevice();
        int RateDevice(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        
        void CreateLogicalDevice();

        void CreateSurface();

        void CreateSwapChain();
        SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int windowWidth, int windowHeight);

        void CreateImageViews();

        void CreateGraphicsPipeline(const char* shaderName);
        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        void CreateRenderPass();

        void CreateFramebuffers();

        void CreateCommandPool();
        void CreateCommandBuffers();

        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

        void CreateSyncObjects();

        void CreateVMesh(const char* meshName);
        void CreateVertexBuffer(VMesh* object);
        void CreateIndexBuffer(VMesh* object);

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t FindMemoryType(uint32_t typeFiler, VkMemoryPropertyFlags properties);

        void CreateDescriptorSetLayout();
        void CreateDescriptorPool();
        void CreateDescriptorSets();

        void CreateUniformBuffers();
        void UpdateUniformBuffer(uint32_t currentImage);

        void CreateVTexture(const char* textureName);
        void CreateTextureImage(VTexture* vTex);
        void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void CreateTextureImageView(VTexture* vTex);
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        void CreateTextureSampler(VTexture* vTex);
        void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

        void CreateDepthResources();
        VkFormat FindDepthFormat();
        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        bool HasStencilComponent(VkFormat format);

        VkSampleCountFlagBits GetMaxUsableSampleCount();
        void CreateColorResources();

        VMesh* GetVMesh(const std::string& name);
        VShader* GetVShader(const std::string& name);

        void CreateRenderable(VMesh* mesh, VShader* vShader);

        bool CheckValidationLayerSupport();
        std::vector<const char*> GetSDLExtensions(SDL_Window* window);

        // Debugging callback
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        void SetupDebugMessenger();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    private:
        SDLModule::SDL2 *sdl2;
        Manager::AssetManager *assetManager;

        const size_t MAX_FRAMES_IN_FLIGHT = 2;
        size_t currentFrame = 0;

        bool framebufferResized = false;

        uint32_t extensionCount = 0;
        std::vector<VkExtensionProperties> extensions;

        VkInstance instance = NULL;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;

        VkSwapchainKHR swapchain;
        std::vector<VkImage> swapchainImages;
        VkFormat swapchainImageFormat;
        VkExtent2D swapchainExtent;

        std::vector<VkImageView> swapchainImageViews;

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkRenderPass renderPass;

        std::vector<VkFramebuffer> swapchainFramebuffers;

        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphore;
        std::vector<VkSemaphore> renderFinishedSemaphore;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;

        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<VkDescriptorSet> descriptorSets;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;

        VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        std::unordered_map<std::string, VMesh> vMeshes;
        std::unordered_map<std::string, VTexture> vTextures;

        std::unordered_map<std::string, VShader> vShaders;
        uint32_t vShaderCount = 0;

        std::unordered_map<uint32_t, Renderable> loadedRenderables;
        uint32_t renderableCount = 0;

        VkDebugUtilsMessengerEXT debugMessenger;
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif
    };
}
