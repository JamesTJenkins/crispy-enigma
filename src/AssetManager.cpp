#include"AssetManager.h"

#include <unordered_map>

#include "Root.h"
#include "OBJLoader.h"
#include "GLTFLoader.h"

namespace Manager {
    AssetManager::AssetManager(Root* _root) : root(_root) {

    }

    AssetManager::~AssetManager() {

    }

    void AssetManager::AddNewTexture(std::string name, std::string path, uint32_t mipLevels) {
        Data::Texture tex;
        tex.texturePath = path;
        tex.mipLevels = mipLevels;

        // Initalize for vulkan
        root->vulkan.vImage.CreateTextureImage(&tex);
        root->vulkan.vImage.CreateTextureImageView(&tex);
        root->vulkan.vImage.CreateTextureSampler(&tex);

        loadedTextures[name] = tex;
    }

    void AssetManager::AddNewShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath) {
        Data::Shader shader;
        shader.vertexShader = vertexShaderPath;
        shader.fragmentShader = fragmentShaderPath;

        // Init for vulkan
        root->vulkan.vPipeline.CreateGraphicsPipeline(&shader);

        loadedShaders[name] = shader;
    }

    void AssetManager::AddNewMaterial(std::string name, std::string textureName, std::string shaderName) {
        Data::Material mat;
        mat.albedo = textureName;
        mat.shader = shaderName;

        loadedMaterials[name] = mat;
    }

    void AssetManager::AddNewMesh(std::string name, std::string path) {
        Data::Mesh mesh;
        mesh.meshPath = path;

        LoadModel(&mesh);

        // Initalize for Vulkan
        root->vulkan.vBuffer.CreateVertexBuffer(&mesh);
        root->vulkan.vBuffer.CreateIndexBuffer(&mesh);

        loadedMeshes[name] = mesh;
    }

    void AssetManager::ClearAssetData() {
        // Destroy all pipelines and clear from asset manager
        root->vulkan.vPipeline.Cleanup();
        loadedShaders.clear();
        // Destroy all textures and clear from asset manager
        root->vulkan.CleanupOldTextures();
        loadedTextures.clear();
        // Destroy all meshes and clear from asset manager
        root->vulkan.CleanupOldMeshes();
        loadedMeshes.clear();
        // Clear from asset manager (only references)
        loadedMaterials.clear();

        std::cout << "Cleared asset data" << std::endl;
    }

    void AssetManager::LoadGLTF(std::string path) {
        std::string extension = GetFileExtension(path);

        if (extension == "glb") {
            Utilities::GLTFLoader loader;
            tinygltf::Model model;

            if (loader.LoadGLB(&model, path)) {
                std::cout << "Loaded gltf file" << std::endl;

                loader.ParseGLTF(root, &model);
            } else {
                std::cout << "Failed to load mesh" << std::endl;
            }
        } else if (extension == "gltf") {
            Utilities::GLTFLoader loader;
            tinygltf::Model model;

            if (loader.LoadGLTF(&model, path)) {
                std::cout << "Loaded gltf file" << std::endl;

                loader.ParseGLTF(root, &model);
            } else {
                std::cout << "Failed to load mesh" << std::endl;
            }
        } else {
            std::cout << "Invalid file" << std::endl;
        }
    }

    std::string AssetManager::GetFileExtension(std::string filePath) {
        if (filePath.find_last_of(".") != std::string::npos)
            return filePath.substr(filePath.find_last_of(".") + 1);

        return "";
    }

    void AssetManager::LoadModel(Data::Mesh* mesh) {
        std::string extension = GetFileExtension(mesh->meshPath);

        if (extension == "obj") {
            LoadObj(mesh);
        } else {
            std::cout << "Invalid file" << std::endl;
        }
    }

    void AssetManager::LoadObj(Data::Mesh* mesh) {
        Utilities::OBJLoader loader;

        if (loader.LoadObj(mesh)) {
            std::cout << "Loaded mesh" << std::endl;
        } else {
            std::cout << "Failed to load mesh" << std::endl;
        }
    }
}