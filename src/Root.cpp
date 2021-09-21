#include "Root.h"

Root::Root() : vulkan(this), assetManager(this), activeScene(this) {
    
}

Root::~Root() {
   
}

uint32_t Root::GetRenderDataSize() {
    // Get the dynamic descriptor size
    uint32_t renderDataSize = 0;

    for (auto rd : renderData) {
        for (auto obj : rd.second) {
            renderDataSize++;
        }
    }

    return renderDataSize;
}