#include "Engine.h"
#include <iostream>
// Testing
#include "Materials.h"
#include "Mesh.h"

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
					root.vulkan.FramebufferResized();
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					root.vulkan.FramebufferResized();
					break;
				case SDL_QUIT:
					Shutdown();
					break;
				default:
					break;
				}
			}

			// Rendering
			Draw();
		}

		Cleanup();
	}

	void Engine::Shutdown() {
		running = false;
	}

	void Engine::Draw(){
		// This is done in an attempt to keep everything as seperated as possible
		root.vulkan.DrawFrame();
	}

	void Engine::Init() {
		running = true;
		
		// Setup sdl2
		root.sdl2.InitSDL2();

		std::cout << "SDL2 Initialized" << std::endl;

		// Load Data
		LoadData();

		// VULKAN
		InitVulkan();

		std::cout << "Vulkan Initialized" << std::endl;
	}

	void Engine::Cleanup() {
		root.vulkan.Cleanup();
		root.sdl2.Cleanup();
	}

	void Engine::LoadData(){
		// TESTING
		root.assetManager.AddNewTexture("test", "assets/textures/test.jpg", 2);
		root.assetManager.AddNewMesh("cube", "assets/models/Cube.obj");
		root.assetManager.AddNewShader("test", "assets/shaders/vert.spv", "assets/shaders/frag.spv");
		root.assetManager.AddNewMaterial("testMat", "test", "test");
	}

	void Engine::InitVulkan() {
		// Setup vulkan
		root.vulkan.InitVulkan(&root.sdl2, &root.assetManager);
		// Create vulkan window
		root.vulkan.CreateVulkanWindow("Test");
		// Create instance
		root.vulkan.CreateInstance();
		// Setup Vulkan Debugger
		root.vulkan.SetupDebugMessenger();
		// Create surface
		root.vulkan.CreateSurface();
		// Select graphics device
		root.vulkan.PickPhysicalDevice();
		// Create logical device
		root.vulkan.CreateLogicalDevice();
		// Create swapchain
		root.vulkan.CreateSwapChain();
		// Create image views
		root.vulkan.CreateImageViews();
		// Create render pass
		root.vulkan.CreateRenderPass();
		// Create descriptor set layout
		root.vulkan.CreateDescriptorSetLayout();

		// Create graphics pipeline for each shader
		for (auto& shader : root.assetManager.loadedShaders){
			root.vulkan.CreateGraphicsPipeline(shader.first.c_str());
		}

		// Create command pool
		root.vulkan.CreateCommandPool();
		// Setup render target
		root.vulkan.CreateColorResources();
		// Create depth resources
		root.vulkan.CreateDepthResources();
		// Create framebuffers
		root.vulkan.CreateFramebuffers();

		// Cycle through all textures
		for (auto& tex : root.assetManager.loadedTextures){
			root.vulkan.CreateVTexture(tex.first.c_str());
		}

		// Cycle through all mesh assets
		for (auto& asset : root.assetManager.loadedMeshes){
			// Create VMeshes
			root.vulkan.CreateVMesh(asset.first.c_str());
		}

		// Create uniform buffer
		root.vulkan.CreateUniformBuffers();
		// Create descriptor pool
		root.vulkan.CreateDescriptorPool();
		// Create descriptor sets
		root.vulkan.CreateDescriptorSets();
		// Create command buffers (FIX)
		root.vulkan.CreateCommandBuffers();
		// Create semaphores and fences
		root.vulkan.CreateSyncObjects();
	}
}