#pragma once
#include "entt.hpp"

namespace Scenes {
    class Scene {
    public:
        Scene();
        ~Scene();

        void LoadData();
    private:
        // Holds component data, and entity ids
        entt::registry mRegistry;
    };
}