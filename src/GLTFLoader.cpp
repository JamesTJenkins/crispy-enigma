#include "GLTFLoader.h"

#include <unordered_map>

#include "Root.h"
#include "Timer.h"

#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION  // set in vulkan image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14

#include "tiny_gltf.h"

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
                
                // Texcoords 
                const tinygltf::Accessor& texcoordAccessor = model->accessors[primitive.attributes["TEXCOORD_0"]];
                const tinygltf::BufferView& texcoordBufferView = model->bufferViews[texcoordAccessor.bufferView];
                const tinygltf::Buffer& texcoordBuffer = model->buffers[texcoordBufferView.buffer];
                const float* texcoords = reinterpret_cast<const float*>(&texcoordBuffer.data[texcoordBufferView.byteOffset + texcoordAccessor.byteOffset]);
                
                // Color
                const tinygltf::Accessor& colorAccessor = model->accessors[primitive.attributes["COLOR_0"]];
                const tinygltf::BufferView& colorBufferView = model->bufferViews[colorAccessor.bufferView];
                const tinygltf::Buffer& colorBuffer = model->buffers[colorBufferView.buffer];
                const float* colors = reinterpret_cast<const float*>(&colorBuffer.data[colorBufferView.byteOffset + colorAccessor.byteOffset]);

                // Indices
                const tinygltf::Accessor& indicesAccessor = model->accessors[primitive.indices];
                const tinygltf::BufferView& indicesBufferView = model->bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer& indicesBuffer = model->buffers[indicesBufferView.buffer];
                const unsigned short* indices = reinterpret_cast<const unsigned short*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);

                for (size_t i = 0; i < posAccessor.count; ++i){
                    Data::Vertex vertex {};
                    
                    vertex.pos = {
                        positions[i * 3 + 0],
                        positions[i * 3 + 1],
                        positions[i * 3 + 2]
                    };

                    vertex.texCoord = {
                        texcoords[i * 2 + 0],
                        1.0f - texcoords[i * 2 + 1]
                    };

                    vertex.color = { 
                        colors[i * 3 + 0], 
                        colors[i * 3 + 1], 
                        colors[i * 3 + 2]
                    };

                    sub.vertices.push_back(vertex);
                }

                sub.indices.resize(indicesAccessor.count);
                for (size_t i = 1; i < indicesAccessor.count; ++i){
                    sub.indices[i] = (uint32_t)indices[i];
                }
                
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
                // Check if given translation, rotation, scale or a matrix
                if (node.translation.size() == 3 && node.rotation.size() == 4 && node.scale.size() == 3) {
                    printf("1");
                    mData.transform = Components::Transform(
                        glm::vec3(node.translation[0], node.translation[1], node.translation[2]), 
                        glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]),  // quat does w,x,y,z while gltf is x,y,z,w
                        glm::vec3(node.scale[0], node.scale[1], node.scale[2])
                    );
                } else if (node.matrix.size() == 16) {
                    printf("2");
                    mData.transform = Components::Transform(glm::mat4(
                        node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3],
                        node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7],
                        node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11],
                        node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]
                    ));
                } else {
                    // If neither just default
                    mData.transform = Components::Transform(glm::vec3(0,0,0), glm::quat(glm::highp_vec3 (0,0,0)), glm::vec3(1,1,1));
                }

                meshData.push_back(mData);
            }
        }
        
        // Generate the entities
        CreateEntities(root, meshData);
    }

    void GLTFLoader::CreateEntities(Root* root, std::vector<MeshData> meshData) {
        // Get current registry
        entt::registry* reg = root->activeScene.GetRegistry();

        // Create entities
        for (size_t i = 0; i < meshData.size(); ++i) {
            entt::entity entity = reg->create();
            reg->emplace<Components::Transform>(entity, meshData[i].transform);
            reg->emplace<Components::MeshRenderer>(entity, meshData[i].meshName, "testMat");
        }
    }

    void GLTFLoader::PrintErrors(std::string* error, std::string* warning) {
        if (!warning->empty()) {
            std::cout << "Warning: " << warning->c_str() << std::endl;
        }

        if (!error->empty()) {
            std::cout << "Error: " << error->c_str() << std::endl;
        }
    }
}