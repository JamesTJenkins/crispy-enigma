#include "GLTFLoader.h"

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

    bool GLTFLoader::LoadGLB(tinygltf::Model *model, std::string path) {
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

    bool GLTFLoader::LoadGLTF(tinygltf::Model *model, std::string path) {
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

    void GLTFLoader::PrintErrors(std::string *error, std::string *warning) {
        if (!warning->empty()) {
            std::cout << "Warning: " << warning->c_str() << std::endl;
        }

        if (!error->empty()) {
            std::cout << "Error: " << error->c_str() << std::endl;
        }
    }
}