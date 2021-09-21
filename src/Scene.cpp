#include "Scene.h"

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
        
    }

    void Scene::CreateAllRenderData(){
        // In here probs due to the first error
        root->renderData.clear();
        auto view = mRegistry.view<Components::MeshRenderer, Components::Transform>();
        
        // Create renderdata for all objects
        for (auto entity : view) {
            VulkanModule::RenderData renderData;
            renderData.meshRenderer = mRegistry.get<Components::MeshRenderer>(entity);
            renderData.transform = mRegistry.get<Components::Transform>(entity).transform;
            root->renderData[root->assetManager.loadedMaterials[renderData.meshRenderer.materialRef].shader].push_back(renderData);
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

        auto v = glm::highp_vec3 (0,0,0);
        auto q = glm::quat(v);

        entt::entity camEntity = mRegistry.create();
        Components::Transform* camTransform = &mRegistry.emplace<Components::Transform>(camEntity, glm::vec3(0,0,-10), q, glm::vec3(1,1,1));
        Components::Camera* cam = &mRegistry.emplace<Components::Camera>(camEntity, (float)root->sdl2.width / root->sdl2.height, glm::quarter_pi<float>(), 0.1f, 100.0f, false, camTransform);
        activeCamera = cam;
        
        entt::entity entity = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity, glm::vec3(0,0,0), q, glm::vec3(1,1,1));
        mRegistry.emplace<Components::MeshRenderer>(entity, "cube", "testMat");

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
        std::cout << root->renderData.size() << " renderable objects loaded" << std::endl;
    }

    int Scene::GetEntityCount(){
        return mRegistry.size();
    }
}