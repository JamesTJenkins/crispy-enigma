#include"Mesh.h"

namespace Data {
    // Tells vulkan how to pass the vertex data
    VkVertexInputBindingDescription Vertex::GetBindingDescription(){
        VkVertexInputBindingDescription bindingDescription{};
        // Index in a binding array
        bindingDescription.binding = 0;
        // Number of bytes per entry
        bindingDescription.stride = sizeof(Vertex);
        // Move the next data entry after each vertex, can be set to instance if instanced rending is needed
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 5> Vertex::GetAttributeDescriptions(){
        std::array<VkVertexInputAttributeDescription, 5> attributeDescription{};
        
        // Position
        attributeDescription[0].binding = 0;
        attributeDescription[0].location = 0;
        // This doesnt mean color channel, use the amount of channels that matches the amount of components
        attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[0].offset = offsetof(Vertex, pos);

        // Normal
        attributeDescription[1].binding = 0;
        attributeDescription[1].location = 1;
        attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[1].offset = offsetof(Vertex, normal);

        // Color
        attributeDescription[2].binding = 0;
        attributeDescription[2].location = 2;
        attributeDescription[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[2].offset = offsetof(Vertex, color);

        // Texcoord 0
        attributeDescription[3].binding = 0;
        attributeDescription[3].location = 3;
        attributeDescription[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescription[3].offset = offsetof(Vertex, texCoord0);

        // Texcoord 1
        attributeDescription[4].binding = 0;
        attributeDescription[4].location = 4;
        attributeDescription[4].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescription[4].offset = offsetof(Vertex, texCoord1);

        return attributeDescription;
    }
}