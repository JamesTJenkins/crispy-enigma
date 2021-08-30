#pragma once
#include <vector>
#include <unordered_map>
#include "Materials.h"
#include "Mesh.h"

/*
    Object manager is just for importing and holding data for Vulkan to use
    Entities are held in the scene that is loaded in 
*/

namespace Manager {
    class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        // Functions
        void AddNewTexture(std::string name, std::string path, uint32_t mipLevels);
        void AddNewShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath);
        void AddNewMaterial(std::string name, std::string textureName, std::string shaderName);
        void AddNewMesh(std::string name, std::string path);

        // Helper functions
        void LoadModel(Data::Mesh* mesh);

        // Objects
        std::unordered_map<std::string, Data::Shader> loadedShaders;
        std::unordered_map<std::string, Data::Texture> loadedTextures;
        std::unordered_map<std::string, Data::Mesh> loadedMeshes;
        std::unordered_map<std::string, Data::Material> loadedMaterials;
    };
}