#include "VulkanSurface.h"
#include <SDL2/SDL_vulkan.h>

#include "Root.h"

namespace VulkanModule {
    VulkanSurface::VulkanSurface(Root* _root, VulkanInstance* _vulkanInstance) : root(_root), vInstance(_vulkanInstance) {

    }

    VulkanSurface::~VulkanSurface() {

    }

    void VulkanSurface::CreateSurface() {
        if (!SDL_Vulkan_CreateSurface(root->sdl2.window, vInstance->instance, &surface)){
			throw std::runtime_error("Failed to create surface.");
		}
    }

    void VulkanSurface::Cleanup() {
        vkDestroySurfaceKHR(vInstance->instance, surface, nullptr);
    }
}