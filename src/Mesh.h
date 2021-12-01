#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <array>
#include <glm.hpp>
#include <gtx/hash.hpp>

namespace Data {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 texCoord0;
        glm::vec2 texCoord1;

        static VkVertexInputBindingDescription GetBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();
    
        bool operator==(const Vertex& other) const {
            return pos == other.pos && normal == other.normal && color == other.color && texCoord0 == other.texCoord0 && texCoord1 == other.texCoord1;
        }
    };

    struct UniformBufferObject {
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct DynamicBufferObject {
        glm::mat4* model;
    };

    struct SubMesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // Vulkan
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
    };

    struct Mesh {
        std::string meshPath;

        std::vector<SubMesh> submeshes;        
    };
}

namespace std {
    template<> struct hash<Data::Vertex> {
        size_t operator()(Data::Vertex const& vertex) const {
            return (
                (hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.normal) << 1 ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord0) << 1) ^
                (hash<glm::vec2>()(vertex.texCoord1) << 1)
            );
        }
    };
}