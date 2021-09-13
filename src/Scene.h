#pragma once
#include <entt.hpp>
#include "Root.fwd.h"
#include "Camera.h"

namespace Scenes {
    class Scene {
    public:
        Scene();
        ~Scene();

        void InitScene(Root* _root);
        void UpdateScene();

        //void RecreateVulkanBuffers();
        void CreateAllRenderData();
        void DrawMeshes();

        void LoadData();
        void PrintLoadedDataStats();
        int GetEntityCount();
    private:
        Root* root;
        Components::Camera* activeCamera;
        // Holds component data, and entity ids
        entt::registry mRegistry;
    };
}