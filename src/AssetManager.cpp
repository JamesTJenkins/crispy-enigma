#include"AssetManager.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>
#include <unordered_map>

namespace Manager {
    AssetManager::AssetManager(){

    }

    AssetManager::~AssetManager(){

    }

    void AssetManager::AddNewTexture(std::string name, std::string path, uint32_t mipLevels){
        Data::Texture tex;
		tex.texturePath = path;
        tex.mipLevels = mipLevels;

        loadedTextures[name] = tex;
    }

    void AssetManager::AddNewShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath){
        Data::Shader shader;
        shader.vertexShader = vertexShaderPath;
        shader.fragmentShader = fragmentShaderPath;

        loadedShaders[name] = shader;
    }

    void AssetManager::AddNewMaterial(std::string name, std::string textureName, std::string shaderName){
        Data::Material mat;
        mat.albedo = textureName;
        mat.shader = shaderName;

        loadedMaterials[name] = mat;
    }

    void AssetManager::AddNewMesh(std::string name, std::string path){
        Data::Mesh mesh;
        mesh.meshPath = path;

        LoadModel(&mesh);

        loadedMeshes[name] = mesh;
    }

    void AssetManager::LoadModel(Data::Mesh* mesh){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mesh->meshPath.c_str())){
            throw std::runtime_error(warn + err);
        }
        
        std::unordered_map<Data::Vertex, uint32_t> uniqueVertices{};

        for(const auto& shape : shapes){
            for(const auto& index : shape.mesh.indices){
                Data::Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                // -1f cause of vulkan having taken it in a different orientation
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0){
                    uniqueVertices[vertex] = static_cast<uint32_t>(mesh->vertices.size());
                    mesh->vertices.push_back(vertex);
                }

                mesh->indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}