#include "Scene.h"
#include "Transform.h"

namespace Scenes {
    Scene::Scene() {
        /*  EXAMPLE OF CREATING ENTITY (https://youtu.be/D4hz0wEB978?t=1589) got here for more stuffs
        entt::entity entity = mRegistry.create();
        mRegistry.emplace<Components::Transform>(entity, glm::vec3(1,1,1));
        */
    }

    Scene::~Scene() {
        
    } 

    void Scene::LoadData(){
        
    }
}