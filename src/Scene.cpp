#include "Scene.h"
#include <chrono>

#include "Transform.h"
#include "MeshRenderer.h"
#include "Root.h"

namespace Scenes {
    Scene::Scene(Root* _root) : root(_root) {
        /*  EXAMPLE OF CREATING ENTITY (https://youtu.be/D4hz0wEB978?t=1589) got here for more stuffs
        entt::entity entity = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity, glm::vec3(1,1,1));
        */
    }

    Scene::~Scene() {
        
    } 

    void Scene::UpdateScene(){
        // Make cubes spin
        auto view = mRegistry.view<Components::MeshRenderer, Components::Transform>();
        
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currTime - startTime).count();

        for (auto entity : view) {
            Components::Transform* transform = &mRegistry.get<Components::Transform>(entity);
            transform->SetRotation(glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

            //std::cout << transform->GetRotation().x << ", " << transform->GetRotation().y << ", " << transform->GetRotation().z << std::endl;
        }

        // Update camera pos
        if (root->inputManager.GetKeyState(SDL_SCANCODE_W)){
            root->activeScene.activeCamera->transformComponent->SetPosition(root->activeScene.activeCamera->transformComponent->GetPosition() + ((glm::vec3(0,0,1) * time) * 0.00025f));
        } else if (root->inputManager.GetKeyState(SDL_SCANCODE_S)){
            root->activeScene.activeCamera->transformComponent->SetPosition(root->activeScene.activeCamera->transformComponent->GetPosition() + ((glm::vec3(0,0,-1) * time) * 0.00025f));
        }

        if (root->inputManager.GetKeyState(SDL_SCANCODE_A)){
            root->activeScene.activeCamera->transformComponent->SetPosition(root->activeScene.activeCamera->transformComponent->GetPosition() + ((glm::vec3(1,0,0) * time) * 0.00025f));
        } else if (root->inputManager.GetKeyState(SDL_SCANCODE_D)){
            root->activeScene.activeCamera->transformComponent->SetPosition(root->activeScene.activeCamera->transformComponent->GetPosition() + ((glm::vec3(-1,0,0) * time) * 0.00025f));
        }

        if (root->inputManager.GetKeyState(SDL_SCANCODE_SPACE)){
            root->activeScene.activeCamera->transformComponent->SetPosition(root->activeScene.activeCamera->transformComponent->GetPosition() + ((glm::vec3(0,1,0) * time) * 0.00025f));
        } else if (root->inputManager.GetKeyState(SDL_SCANCODE_LCTRL)){
            root->activeScene.activeCamera->transformComponent->SetPosition(root->activeScene.activeCamera->transformComponent->GetPosition() + ((glm::vec3(0,-1,0) * time) * 0.00025f));
        }
    }

    void Scene::CreateAllRenderData(){
        root->renderData.clear();
        auto view = mRegistry.view<Components::MeshRenderer, Components::Transform>();
        
        // Create renderdata for all objects
        for (auto entity : view) {
            VulkanModule::RenderData renderData;
            renderData.meshRenderer = &mRegistry.get<Components::MeshRenderer>(entity);
            renderData.transform = &mRegistry.get<Components::Transform>(entity).transform;
            root->renderData[root->assetManager.loadedMaterials[renderData.meshRenderer->materialRef].shader].push_back(renderData);
        }

        // Rebuild Swapchain
        root->vulkan.CleanupOldSwapchain();
        root->vulkan.BuildSwapchain();
    }

    void Scene::LoadData(){
        // Clear all data before loading more
        root->assetManager.ClearAssetData();

        // Load all data for scene
        // TESTING
		root->assetManager.AddNewTexture("test", "assets/textures/test.jpg", 2);
		root->assetManager.AddNewMesh("cube", "assets/models/Cube.obj");
		root->assetManager.LoadGLTF("assets/models/arena.glb");
		root->assetManager.AddNewShader("test", "assets/shaders/vert.spv", "assets/shaders/frag.spv");
		root->assetManager.AddNewMaterial("testMat", "test", "test");

        auto v = glm::highp_vec3 (0,0,0);
        auto q = glm::quat(v);

        entt::entity camEntity = mRegistry.create();
        Components::Transform* camTransform = &mRegistry.emplace<Components::Transform>(camEntity, glm::vec3(0,0,-10), q, glm::vec3(1,1,1));
        Components::Camera* cam = &mRegistry.emplace<Components::Camera>(camEntity, (float)root->sdl2.width / root->sdl2.height, glm::quarter_pi<float>(), 0.1f, 100.0f, false, camTransform);
        activeCamera = cam;
        
        entt::entity entity = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity, glm::vec3(0,0,0), q, glm::vec3(1,1,1));
        mRegistry.emplace<Components::MeshRenderer>(entity, "monkey", "testMat");

        entt::entity entity2 = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity2, glm::vec3(2,2,0), q, glm::vec3(1,1,1));
        mRegistry.emplace<Components::MeshRenderer>(entity2, "cube", "testMat");

        // Rebuild
        // Create all renderdata
        CreateAllRenderData();
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

    int Scene::GetEntityCount(){
        return mRegistry.size();
    }
}