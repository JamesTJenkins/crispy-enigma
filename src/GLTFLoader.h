#pragma once
#include <iostream>

// Tempory fix
// Cant seem to get the required defines working here without linker errors
#include "tiny_gltf.h"
#include "Root.fwd.h"

namespace Utilities {
    class GLTFLoader {
    public:
        // Constructors
        GLTFLoader();
        ~GLTFLoader();

        // Functions
        bool LoadGLB(tinygltf::Model *model, std::string path);
        bool LoadGLTF(tinygltf::Model *model, std::string path);
  
        void ParseGLTF(Root* root, tinygltf::Model *model);
    private:
        // Helper functions
        void PrintErrors(std::string *error, std::string *warning);
    };
}