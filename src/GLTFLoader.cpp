#include "GLTFLoader.h"

#include <unordered_map>

#include "Root.h"
#include "Timer.h"

#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION  // set in vulkan image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14

#include "tiny_gltf.h"
#include <gtx/matrix_decompose.hpp>
#include <cstdint>

namespace Utilities {
    GLTFLoader::GLTFLoader() {

    }

    GLTFLoader::~GLTFLoader() {

    }

    bool GLTFLoader::LoadGLB(tinygltf::Model* model, std::string path) {
        tinygltf::TinyGLTF loader;

        std::string error;
        std::string warning;

        Utilities::Timer timer;
        timer.Start();

        if (loader.LoadBinaryFromFile(model, &error, &warning, path)) {
            PrintErrors(&error, &warning);

            timer.Stop();

            std::cout << "File parsing time: " << timer.ElapsedMilliseconds() << "ms" << std::endl;

            return true;
        } else {
            PrintErrors(&error, &warning);

            timer.Stop();

            std::cout << "File parsing time: " << timer.ElapsedMilliseconds() << "ms" << std::endl;

            return false;
        }
    }

    bool GLTFLoader::LoadGLTF(tinygltf::Model* model, std::string path) {
        tinygltf::TinyGLTF loader;

        std::string error;
        std::string warning;

        Utilities::Timer timer;
        timer.Start();

        if (loader.LoadASCIIFromFile(model, &error, &warning, path)) {
            PrintErrors(&error, &warning);

            timer.Stop();

            std::cout << "File loading time: " << timer.ElapsedMilliseconds() << "ms" << std::endl;

            return true;
        } else {
            PrintErrors(&error, &warning);

            timer.Stop();

            std::cout << "File loading time: " << timer.ElapsedMilliseconds() << "ms" << std::endl;

            return false;
        }
    }

    void GLTFLoader::ParseGLTF(Root* root, tinygltf::Model* model) {
        // Containers
        std::vector<MeshData> meshData;

        // Load all meshes
        std::cout << "Processing meshes:" << std::endl;

        for (auto& m : model->meshes) {
            Utilities::Timer timer;
            timer.Start();

            Data::Mesh mesh;

            for (auto& primitive : m.primitives) {
                Data::SubMesh sub;

                // Vertices
                const tinygltf::Accessor& posAccessor = model->accessors[primitive.attributes["POSITION"]];
                const tinygltf::BufferView& posBufferView = model->bufferViews[posAccessor.bufferView];
                const tinygltf::Buffer& posBuffer = model->buffers[posBufferView.buffer];
                const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);
                
                // Normals
                const tinygltf::Accessor& normalAccessor = model->accessors[primitive.attributes["NORMAL"]];
                const tinygltf::BufferView& normalBufferView = model->bufferViews[normalAccessor.bufferView];
                const tinygltf::Buffer& normalBuffer = model->buffers[normalBufferView.buffer];
                const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);

                // Texcoord 0
                const tinygltf::Accessor& texcoord0Accessor = model->accessors[primitive.attributes["TEXCOORD_0"]];
                const tinygltf::BufferView& texcoord0BufferView = model->bufferViews[texcoord0Accessor.bufferView];
                const tinygltf::Buffer& texcoord0Buffer = model->buffers[texcoord0BufferView.buffer];
                const float* texcoord0 = reinterpret_cast<const float*>(&texcoord0Buffer.data[texcoord0BufferView.byteOffset + texcoord0Accessor.byteOffset]);
                
                // Texcoord 1
                const tinygltf::Accessor& texcoord1Accessor = model->accessors[primitive.attributes["TEXCOORD_1"]];
                const tinygltf::BufferView& texcoord1BufferView = model->bufferViews[texcoord1Accessor.bufferView];
                const tinygltf::Buffer& texcoord1Buffer = model->buffers[texcoord1BufferView.buffer];
                const float* texcoord1 = reinterpret_cast<const float*>(&texcoord1Buffer.data[texcoord1BufferView.byteOffset + texcoord1Accessor.byteOffset]);

                // Color
                const tinygltf::Accessor& colorAccessor = model->accessors[primitive.attributes["COLOR_0"]];
                const tinygltf::BufferView& colorBufferView = model->bufferViews[colorAccessor.bufferView];
                const tinygltf::Buffer& colorBuffer = model->buffers[colorBufferView.buffer];
                const float* colors = reinterpret_cast<const float*>(&colorBuffer.data[colorBufferView.byteOffset + colorAccessor.byteOffset]);

                for (size_t i = 0; i < posAccessor.count; ++i){
                    Data::Vertex vertex {};

                    vertex.pos = {
                        positions[i * 3 + 0],
                        positions[i * 3 + 1],
                        positions[i * 3 + 2]
                    };

                    vertex.normal = {
                        normals[i * 3 + 0],
                        normals[i * 3 + 1],
                        normals[i * 3 + 2]
                    };

                    vertex.texCoord0 = {
                        texcoord0[i * 2 + 0],
                        1.0f - texcoord0[i * 2 + 1]
                    };

                    vertex.texCoord1 = {
                        texcoord1[i * 2 + 0],
                        1.0f - texcoord1[i * 2 + 1]
                    };

                    vertex.color = { 
                        colors[i * 3 + 0], 
                        colors[i * 3 + 1], 
                        colors[i * 3 + 2]
                    };

                    sub.vertices.push_back(vertex);
                }

                // Indices
                const tinygltf::Accessor& indicesAccessor = model->accessors[primitive.indices];
                const tinygltf::BufferView& indicesBufferView = model->bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer& indicesBuffer = model->buffers[indicesBufferView.buffer];

                if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                    const unsigned short* indices = reinterpret_cast<const unsigned short*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
                
                    sub.indices.resize(indicesAccessor.count);
                    for (size_t i = 0; i < indicesAccessor.count; ++i){
                        sub.indices[i] = (uint32_t)indices[i];
                    }
                } else if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                    const unsigned int* indices = reinterpret_cast<const unsigned int*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
                    
                    sub.indices.resize(indicesAccessor.count);
                    for (size_t i = 0; i < indicesAccessor.count; ++i){
                        sub.indices[i] = (uint32_t)indices[i];
                    }
                } else if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                    const uint8_t* indices = reinterpret_cast<const uint8_t*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
                    
                    sub.indices.resize(indicesAccessor.count);
                    for (size_t i = 0; i < indicesAccessor.count; ++i){
                        sub.indices[i] = (uint32_t)indices[i];
                    }
                }
                
                // Add submesh
                mesh.submeshes.push_back(sub);
            }

            // Initalize for Vulkan
            root->vulkan.vBuffer.CreateVertexBuffer(&mesh);
            root->vulkan.vBuffer.CreateIndexBuffer(&mesh);

            root->assetManager.loadedMeshes[m.name] = mesh;

            timer.Stop();

            std::cout << m.name << " parsed in: " << timer.ElapsedMilliseconds() << "ms" << std::endl;
        }

        // Load all nodes
        std::cout << "Processing GLTF nodes:" << std::endl;
        
        for (auto& node : model->nodes) {
            if (node.mesh != -1) {
                std::cout << "Found mesh data for object: " << node.name << std::endl;

                MeshData mData;

                mData.meshName = model->meshes[node.mesh].name;
                
                glm::vec3 translation;
                glm::quat rotation;
                glm::vec3 scale;

                bool translation_provided = node.translation.size() >= 3,
                    rotation_provided = node.rotation.size() >= 4, 
                    scale_provided = node.scale.size() >= 3,
                    matrix_provided = node.matrix.size() >= 16;

                if (!matrix_provided) {
                    
                    if (translation_provided)
                        translation = DoubleToVec3(node.translation);
                    else translation = {0,0,0};

                    if (rotation_provided)
                        rotation = DoubleToQuat (node.rotation);
                    else rotation = {1,0,0,0};

                    if (scale_provided)
                        scale = DoubleToVec3(node.scale);
                    else scale = {1,1,1};

                } else {
                    auto matrix = DoubleToMat4(node.matrix);
                    glm::vec3 skew;
                    glm::vec4 perspective;
                    glm::decompose(matrix, scale, rotation, translation, skew, perspective);
                }

                mData.transform = Components::Transform(translation, rotation, scale);
                meshData.push_back(mData);
            }
        }
        
        // Generate the entities
        CreateEntities(root, meshData);
    }

    // Entity creation

    void GLTFLoader::CreateEntities(Root* root, std::vector<MeshData> meshData) {
        // Get current registry
        entt::registry* reg = root->activeScene.GetRegistry();

        // Create entities
        for (size_t i = 0; i < meshData.size(); ++i) {
            entt::entity entity = reg->create();
            reg->emplace<Components::Transform>(entity, meshData[i].transform);
            // TODO: make it go off the gltf loader materials
            reg->emplace<Components::MeshRenderer>(entity, meshData[i].meshName, "testMat");
        }
    }

    // Conversions from tinygltf double vectors to glm types

    glm::vec3 GLTFLoader::DoubleToVec3(std::vector<double> doubles) {
        return glm::vec3(doubles[0], doubles[1], doubles[2]);
    }

    glm::quat GLTFLoader::DoubleToQuat(std::vector<double> doubles) {
        // quat does w,x,y,z while gltf is x,y,z,w
        return glm::quat(doubles[3], doubles[0], doubles[1], doubles[2]);
    }

    glm::mat4 GLTFLoader::DoubleToMat4(std::vector<double> doubles) {
        return glm::mat4(
            doubles[0], doubles[1], doubles[2], doubles[3],
            doubles[4], doubles[5], doubles[6], doubles[7],
            doubles[8], doubles[9], doubles[10], doubles[11],
            doubles[12], doubles[13], doubles[14], doubles[15]
        );
    }

    // Debugging

    void GLTFLoader::PrintErrors(std::string* error, std::string* warning) {
        if (!warning->empty()) {
            std::cout << "Warning: " << warning->c_str() << std::endl;
        }

        if (!error->empty()) {
            std::cout << "Error: " << error->c_str() << std::endl;
        }
    }
}