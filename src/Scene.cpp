#include "Scene.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "Root.h"

namespace Scenes {
    Scene::Scene() {
        /*  EXAMPLE OF CREATING ENTITY (https://youtu.be/D4hz0wEB978?t=1589) got here for more stuffs
        entt::entity entity = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity, glm::vec3(1,1,1));
        */
    }

    Scene::~Scene() {
        
    } 

    void Scene::InitScene(Root* _root){
        root = _root;
    }

    void Scene::UpdateScene(){
        
    }

    void Scene::CreateAllRenderData(){
        auto view = mRegistry.view<Components::MeshRenderer, Components::Transform>();
        
        // Create renderdata for all objects
        for (auto entity : view) {
            //root->vulkan.AddRenderData(&mRegistry.get<Components::MeshRenderer>(entity), &mRegistry.get<Components::Transform>(entity));
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
		root->assetManager.AddNewShader("test", "assets/shaders/vert.spv", "assets/shaders/frag.spv");
		root->assetManager.AddNewMaterial("testMat", "test", "test");

        entt::entity camEntity = mRegistry.create();
        Components::Transform camTransform = mRegistry.emplace<Components::Transform>(camEntity);
        Components::Camera cam = mRegistry.emplace<Components::Camera>(camEntity, 1920/1080, 45.0f, 0.1f, 100.0f, false, &camTransform);
        activeCamera = &cam;
        
        entt::entity entity = mRegistry.create();
        Components::MeshRenderer mr = mRegistry.emplace<Components::MeshRenderer>(entity, "cube", "testMat");
        mRegistry.emplace<Components::Transform>(entity);


        // Rebuild
        // Clear any data being held first
        //root->vulkan.ClearRenderData();
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
    }

    int Scene::GetEntityCount(){
        return mRegistry.size();
    }
}