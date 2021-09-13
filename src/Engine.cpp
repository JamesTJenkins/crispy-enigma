#include "Engine.h"
#include <iostream>

namespace Engine {
	Engine::Engine() {

	}

	Engine::~Engine() {

	}

	void Engine::Run(const Game& game) {
		Init();

		while (running) {
			// Engine loop
			SDL_Event event;
			// Poll until all events have been handled
			while (SDL_PollEvent (&event)) {
				// What happens to each poll
				switch (event.type) {
				case SDL_WINDOWEVENT_RESIZED:
					root.vulkan.vFramebuffer.FramebufferResized();
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					root.vulkan.vFramebuffer.FramebufferResized();
					break;
				case SDL_QUIT:
					Shutdown();
					break;
				default:
					break;
				}
			}

			// Scene update
			root.activeScene.UpdateScene();
			// Rendering
			root.vulkan.DrawFrame();
		}

		Cleanup();
	}

	void Engine::Shutdown() {
		running = false;
	}

	void Engine::Init() {
		running = true;
		
		// Setup sdl2
		root.sdl2.InitSDL2();

		// VULKAN
		root.vulkan.InitVulkan();

		// Load Data
		root.activeScene.LoadData();

		std::cout << "Loaded Scene, Entity count: " << root.activeScene.GetEntityCount() << std::endl;
	}

	void Engine::Cleanup() {
		root.vulkan.Cleanup();
		root.sdl2.Cleanup();
	}
}