#pragma once
#include <iostream>
#include <vector>
#include<vulkan/vulkan.h>

namespace Materials {
    struct Texture {
        int width;
        int height;
        int channels;
        uint32_t mipLevels;
        std::string texturePath;
    };

    struct Shader {
        std::string vertexShader;
        std::string fragmentShader;

        static std::vector<char> ReadBinaryFile(const std::string& filename);
    };

    struct Material {
        std::string albedo;
        std::string shader;
    };
}