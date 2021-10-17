#include "GLTFLoader.h"

#include <unordered_map>

#include "Root.h"

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

        if (loader.LoadBinaryFromFile(model, &error, &warning, path)) {
            PrintErrors(&error, &warning);

            return true;
        } else {
            PrintErrors(&error, &warning);

            return false;
        }
    }

    bool GLTFLoader::LoadGLTF(tinygltf::Model* model, std::string path) {
        tinygltf::TinyGLTF loader;

        std::string error;
        std::string warning;

        if (loader.LoadASCIIFromFile(model, &error, &warning, path)) {
            PrintErrors(&error, &warning);

            return true;
        } else {
            PrintErrors(&error, &warning);

            return false;
        }
    }

    void GLTFLoader::ParseGLTF(Root* root, tinygltf::Model* model) {
        // Meshes
        for (auto& mesh : model->meshes) {
            Data::Mesh m;

            for (auto& primitive : mesh.primitives) {
                Data::SubMesh sub;
                std::unordered_map<Data::Vertex, uint32_t> uniqueVertices {};
              
                // Vertices
                const tinygltf::Accessor& posAccessor = model->accessors[primitive.attributes["POSITION"]];
                const tinygltf::BufferView& posBufferView = model->bufferViews[posAccessor.bufferView];
                const tinygltf::Buffer& posBuffer = model->buffers[posBufferView.buffer];
                const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);
                
                // Texcoords 
                const tinygltf::Accessor& texcoordAccessor = model->accessors[primitive.attributes["TEXCOORD_0"]];
                const tinygltf::BufferView& texcoordBufferView = model->bufferViews[texcoordAccessor.bufferView];
                const tinygltf::Buffer& texcoordBuffer = model->buffers[texcoordBufferView.buffer];
                const glm::vec2* texcoords = reinterpret_cast<const glm::vec2*>(&texcoordBuffer.data[texcoordBufferView.byteOffset + texcoordAccessor.byteOffset]);
                
                // Indices
                const tinygltf::Accessor& indicesAccessor = model->accessors[primitive.indices];
                const tinygltf::BufferView& indicesBufferView = model->bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer& indicesBuffer = model->buffers[indicesBufferView.buffer];
                const int* indices = reinterpret_cast<const int*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);

                for (size_t i = 0; i < indicesAccessor.count; ++i) {
                    const int index = indices[i];
                    Data::Vertex vertex {};
                    
                    printf("curr index %d \n", index);
                    // Something is going horribly wrong with the index when being used
                    vertex.pos = {
                        positions[index * 3 + 0],
                        positions[index * 3 + 1],
                        positions[index * 3 + 2]
                    };
                    
                    vertex.texCoord = {
                        texcoords[index].x,
                        1.0f - texcoords[index].y
                    };
                    
                    vertex.color = { 1.0f, 1.0f, 1.0f };
                    
                    if (uniqueVertices.count(vertex) == 0) {
                        uniqueVertices[vertex] = static_cast<uint32_t>(sub.vertices.size());
                        sub.vertices.push_back(vertex);
                    }
                    
                    sub.indices.push_back(uniqueVertices[vertex]);
                }
            }

            // Initalize for Vulkan
            root->vulkan.vBuffer.CreateVertexBuffer(&m);
            root->vulkan.vBuffer.CreateIndexBuffer(&m);

            root->assetManager.loadedMeshes[mesh.name] = m;
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