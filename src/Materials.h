#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

namespace Data {
    struct Texture {
        int width;
        int height;
        int channels;
        uint32_t mipLevels;
        std::string texturePath;

        // Vulkan
        int textureId;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkSampler textureSampler;
        VkImageView textureImageView;
    };

    struct Shader {
        std::string vertexShader;
        std::string fragmentShader;

        // Vulkan
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;

        static std::vector<char> ReadBinaryFile(const std::string& filename);
    };

    struct Material {
        std::string texture;
        float specular;
        float shininess;

        std::string shader;
    };
}