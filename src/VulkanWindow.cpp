#include "VulkanWindow.h"
#include <SDL2/SDL_vulkan.h>

#include "Root.h"

namespace VulkanModule {
    VulkanWindow::VulkanWindow(Root* _root) : root(_root) {

    }

    VulkanWindow::~VulkanWindow() {

    }

    void VulkanWindow::CreateVulkanWindow(const char *title) {
		// Creates an SDL window with correct flags (RESIZABLE FLAG IS THERE FOR TESTING PURPOSES)
		root->sdl2.CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, root->sdl2.width, root->sdl2.height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    }
}