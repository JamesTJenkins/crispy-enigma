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

    std::array<VkVertexInputAttributeDescription, 3> Vertex::GetAttributeDescriptions(){
        std::array<VkVertexInputAttributeDescription, 3> attributeDescription{};
        
        attributeDescription[0].binding = 0;
        attributeDescription[0].location = 0;
        // This doesnt mean color channel, use the amount of channels that matches the amount of components
        attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[0].offset = offsetof(Vertex, pos);

        attributeDescription[1].binding = 0;
        attributeDescription[1].location = 1;
        attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[1].offset = offsetof(Vertex, color);

        attributeDescription[2].binding = 0;
        attributeDescription[2].location = 2;
        attributeDescription[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescription[2].offset = offsetof(Vertex, texCoord);

        return attributeDescription;
    }
}