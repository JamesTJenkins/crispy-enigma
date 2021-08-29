#include"ObjectManager.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>
#include <unordered_map>

namespace Manager {
    ObjectManager::ObjectManager(){

    }

    ObjectManager::~ObjectManager(){

    }

    void ObjectManager::AddNewTexture(std::string name, std::string path, uint32_t mipLevels){
        Materials::Texture tex;
		tex.texturePath = path;
        tex.mipLevels = mipLevels;

        loadedTextures[name] = tex;
    }

    void ObjectManager::AddNewShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath){
        Materials::Shader shader;
        shader.vertexShader = vertexShaderPath;
        shader.fragmentShader = fragmentShaderPath;

        loadedShaders[name] = shader;
    }

    void ObjectManager::AddNewMaterial(std::string name, std::string textureName, std::string shaderName){
        Materials::Material mat;
        mat.albedo = textureName;
        mat.shader = shaderName;

        loadedMaterials[name] = mat;
    }

    void ObjectManager::AddNewMeshObject(std::string name, std::string path){
        Mesh::MeshObject obj;
        obj.objectPath = path;

        LoadModel(&obj);

        loadedMeshObjects[name] = obj;
    }

    void ObjectManager::AddObject(std::string meshName, std::string materialName){
        Object obj;
        obj.mesh = meshName;
        obj.material = materialName;

        loadedObjects.push_back(obj);
    }

    void ObjectManager::LoadModel(Mesh::MeshObject* meshObject){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshObject->objectPath.c_str())){
            throw std::runtime_error(warn + err);
        }
        
        std::unordered_map<Mesh::Vertex, uint32_t> uniqueVertices{};

        for(const auto& shape : shapes){
            for(const auto& index : shape.mesh.indices){
                Mesh::Vertex vertex{};

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
                    uniqueVertices[vertex] = static_cast<uint32_t>(meshObject->vertices.size());
                    meshObject->vertices.push_back(vertex);
                }

                meshObject->indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}