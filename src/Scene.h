#pragma once
#include <entt.hpp>
#include "Root.fwd.h"
#include "Camera.h"

namespace Scenes {
    class Scene {
    public:
        Scene(Root* _root);
        ~Scene();
        
        void UpdateScene();

        void CreateAllRenderData();
        void DrawMeshes();

        void LoadData();
        void PrintLoadedDataStats();
        int GetEntityCount();

        Components::Camera* activeCamera = nullptr;
    private:
        Root* root;
        // Holds component data, and entity ids
        entt::registry mRegistry;
    };
}