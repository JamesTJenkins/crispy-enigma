#pragma once
#include <vector>
#include <unordered_map>

#include "Materials.h"
#include "Mesh.h"
#include "Root.fwd.h"
#include "tiny_gltf.h"

/*
    Object manager is just for importing and holding data for Vulkan to use
    Entities are held in the scene that is loaded in 
*/

namespace Manager {
    class AssetManager {
    public:
        AssetManager(Root* _root);
        ~AssetManager();

        // Functions
        void AddNewTexture(std::string name, std::string path, uint32_t mipLevels);
        void AddNewShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath);
        void AddNewMaterial(std::string name, std::string textureName, std::string shaderName);
        void AddNewMesh(std::string name, std::string path);
        void LoadGLTF(std::string path);

        // Helper functions
        void ClearAssetData();

        // Objects
        std::unordered_map<std::string, Data::Shader> loadedShaders;
        std::unordered_map<std::string, Data::Texture> loadedTextures;
        std::unordered_map<std::string, Data::Mesh> loadedMeshes;
        std::unordered_map<std::string, Data::Material> loadedMaterials;
    private:
        std::string GetFileExtension(std::string filePath);
        
        void LoadModel(Data::Mesh* mesh);
        void LoadObj(Data::Mesh* mesh);
        
        Root* root;
    };
}