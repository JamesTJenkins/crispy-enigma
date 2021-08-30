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

    // SDL2
    SDLModule::SDL2 sdl2;
    // Vulkan
    VulkanModule::Vulkan vulkan;
    // Object manager
    Manager::AssetManager assetManager;
    // Loaded scene
    Scenes::Scene activeScene;
};