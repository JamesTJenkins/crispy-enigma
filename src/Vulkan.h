#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Root.fwd.h"
#include "VulkanWindow.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptor.h"
#include "VulkanFrameBuffer.h"
#include "VulkanCommand.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanResources.h"
#include "VulkanUniformBuffer.h"
#include "VulkanSync.h"
#include "VulkanPipeline.h"

#include "MeshRenderer.h"

/*
    Where all the parts of my vulkan implementation come together
*/

namespace VulkanModule {
    struct RenderData {
        Components::MeshRenderer meshRenderer;
        glm::mat4 transform;
    };

    class Vulkan {
    private:
        // This is all done first as it requires to be initalized first
        // References
        Root* root;

        // Extensions
        uint32_t extensionCount = 0;
        std::vector<VkExtensionProperties> extensions;

        // Frames
        const size_t MAX_FRAMES_IN_FLIGHT = 2;
        size_t currentFrame = 0;
    
    public:
        // Constructors
        Vulkan(Root* _root);
        ~Vulkan();

        // Initiate vulkan
        void InitVulkan();
        // Draw frame
        void DrawFrame();

        // Set window size (would have put in vulkan window if i didnt have to add framebuffer to it)
        void SetWindowSize(int width, int height);
        // If window changes
        void WindowChanged();

        // Create swapchain
        void BuildSwapchain();
        // Cleanup old swapchain
        void CleanupOldSwapchain();
        // Cleanup all of vulkan
        void Cleanup();
        // Cleanup old textures
        void CleanupOldTextures();
        // Cleanup old meshes
        void CleanupOldMeshes();

        // Vulkan components (think it break if i move these lines around)
        VulkanWindow vWindow;
        VulkanInstance vInstance;
        VulkanSurface vSurface;
        VulkanDevice vDevice;
        VulkanSwapchain vSwapchain;
        VulkanImageView vImageView;
        VulkanRenderPass vRenderPass;
        VulkanDescriptor vDescriptor;
        VulkanFrameBuffer vFramebuffer;
        VulkanCommand vCommandbuffer;
        VulkanBuffer vBuffer;
        VulkanImage vImage;
        VulkanResources vResources;
        VulkanUniformBuffer vUniformBuffer;
        VulkanSync vSync;
        VulkanPipeline vPipeline;
    };
}