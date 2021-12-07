#include "Scene.h"
#include <chrono>

#include "Transform.h"
#include "MeshRenderer.h"
#include "Root.h"

namespace Scenes {
    Scene::Scene(Root* _root) : root(_root) {
        // mRegistry gets made automatically
    }

    Scene::~Scene() {
        
    } 

    void Scene::UpdateScene(){
        // Make cubes spin
        auto view = mRegistry.view<Components::MeshRenderer, Components::Transform>();
        
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currTime - startTime).count();

        /*
        for (auto entity : view) {
            Components::Transform* transform = &mRegistry.get<Components::Transform>(entity);
            transform->SetRotation(glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

            //std::cout << transform->GetRotation().x << ", " << transform->GetRotation().y << ", " << transform->GetRotation().z << std::endl;
        }
        */

       Components::Transform* cam = root->activeScene.activeCamera->transformComponent;

        // Update camera pos
        if (root->inputManager.GetKeyState(SDL_SCANCODE_W)){
            cam->SetPosition(cam->GetPosition() + (time * cam->Forward()) * 0.001f);
        } else if (root->inputManager.GetKeyState(SDL_SCANCODE_S)){
            cam->SetPosition(cam->GetPosition() + (time * cam->Backward()) * 0.001f);
        }

        if (root->inputManager.GetKeyState(SDL_SCANCODE_A)){
            cam->SetPosition(cam->GetPosition() + (time * cam->Left()) * 0.001f);
        } else if (root->inputManager.GetKeyState(SDL_SCANCODE_D)){
            cam->SetPosition(cam->GetPosition() + (time * cam->Right()) * 0.001f);
        }

        if (root->inputManager.GetKeyState(SDL_SCANCODE_SPACE)){
            cam->SetPosition(cam->GetPosition() + (time * cam->Up()) * 0.001f);
        } else if (root->inputManager.GetKeyState(SDL_SCANCODE_LCTRL)){
            cam->SetPosition(cam->GetPosition() + (time * cam->Down()) * 0.001f);
        }
        
        static int prevMouseX = 0;
        static int prevMouseY = 0;
        int mouseX = root->inputManager.GetMouseX();
        int mouseY = root->inputManager.GetMouseY();

        if (mouseX > prevMouseX) {
            // Right
            cam->Rotate(time * 0.1f * glm::radians(-1.0f), glm::vec3(0,1,0));
        } else if (mouseX < prevMouseX) {
            // Left
            cam->Rotate(time * 0.1f * glm::radians(1.0f), glm::vec3(0,1,0));
        }

        if (mouseY > prevMouseY) {
            // Up
            cam->Rotate(time * 0.1f * glm::radians(-1.0f), cam->Right());
        } else if (mouseY < prevMouseY) {
            // Down
            cam->Rotate(time * 0.1f * glm::radians(1.0f), cam->Right());
        }

        prevMouseX = mouseX;
        prevMouseY = mouseY;
    }

    void Scene::CreateAllRenderData(){
        root->renderData.clear();
        auto view = mRegistry.view<Components::MeshRenderer, Components::Transform>();
        
        // Create renderdata for all objects
        for (auto entity : view) {
            VulkanModule::RenderData renderData;
            renderData.meshRenderer = &mRegistry.get<Components::MeshRenderer>(entity);
            renderData.transform = mRegistry.get<Components::Transform>(entity).LocalToWorldMatrix();
            root->renderData[root->assetManager.loadedMaterials[renderData.meshRenderer->materialRef].shader].push_back(renderData);
        }

        // Rebuild Swapchain
        root->vulkan.CleanupOldSwapchain();
        root->vulkan.BuildSwapchain();
    }

    void Scene::UpdateLightData() {
        root->lights.clear();
        auto view = mRegistry.view<Components::Light>();
        
        // Get all lights and sort them based off type
        for (auto entity : view) {
            Components::Light* light = &mRegistry.get<Components::Light>(entity);
            root->lights[light->lightType].push_back(light);
        }
    }

    void Scene::LoadData(){
        // Clear all data before loading more
        root->assetManager.ClearAssetData();

        // Load all data for scene
        // TESTING
		root->assetManager.AddNewTexture("test", "assets/textures/test.jpg", 2);
		root->assetManager.AddNewTexture("test2", "assets/textures/test2.jpg", 2);
		root->assetManager.LoadGLTF("assets/models/arena.glb");
		root->assetManager.AddNewShader("test", "assets/shaders/vert.spv", "assets/shaders/frag.spv");
		root->assetManager.AddNewMaterial("testMat", "test", 0.0f, 16.0f, "test");

        // Default values
        auto v = glm::highp_vec3 (0,0,0);
        auto q = glm::quat(v);

        // Camera
        entt::entity camEntity = mRegistry.create();
        Components::Transform* camTransform = &mRegistry.emplace<Components::Transform>(camEntity, glm::vec3(0,0,-10), q, glm::vec3(1,1,1));
        Components::Camera* cam = &mRegistry.emplace<Components::Camera>(camEntity, (float)root->sdl2.width / root->sdl2.height, glm::quarter_pi<float>(), 0.1f, 1000.0f, false, camTransform);
        activeCamera = cam;
        
        // Light
        entt::entity lightEntity = mRegistry.create();
        mRegistry.emplace<Components::Light>(lightEntity, Components::DIRECTIONAL, glm::vec3(0,10,0), glm::vec3(1,1,1), 10);

        /* CUBE TEST
		root->assetManager.AddNewMesh("cube", "assets/models/Cube.obj");
		root->assetManager.AddNewMaterial("testMat2", "test2", "test");

        entt::entity entity = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity, glm::vec3(0,0,0), q, glm::vec3(1,1,1));
        mRegistry.emplace<Components::MeshRenderer>(entity, "cube", "testMat2");
        */

        // Rebuild
        // Create all renderdata
        CreateAllRenderData();
        // Update lighting
        UpdateLightData();
        // Print off stats
        PrintLoadedDataStats();
    }

    void Scene::PrintLoadedDataStats() {
        std::cout << root->assetManager.loadedTextures.size() << " texture(s) loaded" << std::endl;
        std::cout << root->assetManager.loadedMeshes.size() << " mesh(es) loaded" << std::endl;
        std::cout << root->assetManager.loadedShaders.size() << " shader(s) loaded" << std::endl;
        std::cout << root->assetManager.loadedMaterials.size() << " material(s) loaded" << std::endl;
        std::cout << root->GetRenderDataSize() << " renderable objects loaded" << std::endl;
    }

    entt::registry* Scene::GetRegistry() {
        return &mRegistry;
    }

    int Scene::GetEntityCount(){
        return mRegistry.size();
    }
}