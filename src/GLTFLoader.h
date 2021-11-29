#pragma once
#include <iostream>

#include "Transform.h"
#include "tiny_gltf.h"
#include "Root.fwd.h"

struct MeshData {
    std::string meshName;
    Components::Transform transform;
};

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
        glm::vec3 DoubleToVec3(std::vector<double> doubles);
        glm::quat DoubleToQuat(std::vector<double> doubles);
        glm::mat4 DoubleToMat4(std::vector<double> doubles);

        void CreateEntities(Root* root, std::vector<MeshData> meshData);
        void PrintErrors(std::string *error, std::string *warning);
    };
}