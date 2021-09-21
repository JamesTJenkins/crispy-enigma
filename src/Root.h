#pragma once
#include <cstdint>
#include "Vulkan.h"
#include "SDL2.h"
#include "AssetManager.h"
#include "Scene.h"

class Root {
public:
    Root();
    ~Root();

    uint32_t GetRenderDataSize();

    // SDL2
    SDLModule::SDL2 sdl2;
    // Vulkan
    VulkanModule::Vulkan vulkan;
    std::unordered_map<std::string, std::vector<VulkanModule::RenderData>> renderData;
    // Object manager
    Manager::AssetManager assetManager;
    // Loaded scene
    Scenes::Scene activeScene;
};