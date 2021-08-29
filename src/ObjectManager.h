#pragma once
#include <vector>
#include <unordered_map>
#include "Materials.h"
#include "Mesh.h"

/*
    This object manager is based off names as ids for individual objects, and the referencing outside of vulkan would just be with names.
    Hopefully should be quite freindly and useful as saving objects in json would simply be saving a string.
    Also should be quite clear when an error occurs
*/

namespace Manager {
    struct Object {
        std::string mesh;
        std::string material;
    };

    class ObjectManager {
    public:
        ObjectManager();
        ~ObjectManager();

        // Functions
        void AddNewTexture(std::string name, std::string path, uint32_t mipLevels);
        void AddNewShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath);
        void AddNewMaterial(std::string name, std::string textureName, std::string shaderName);
        void AddNewMeshObject(std::string name, std::string path);
        void AddObject(std::string meshName, std::string materialName);

        // Helper functions
        void LoadModel(Mesh::MeshObject* meshObject);

        // Objects
        std::vector<Object> loadedObjects;
        std::unordered_map<std::string, Materials::Shader> loadedShaders;
        std::unordered_map<std::string, Materials::Texture> loadedTextures;
        std::unordered_map<std::string, Mesh::MeshObject> loadedMeshObjects;
        std::unordered_map<std::string, Materials::Material> loadedMaterials;
    };
}