#include "OBJLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <tiny_obj_loader.h>

#include "Timer.h"

namespace Utilities {
    OBJLoader::OBJLoader() {

    }

    OBJLoader::~OBJLoader() {

    }

    bool OBJLoader::LoadObj(Data::Mesh *mesh) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        Utilities::Timer timer;
        timer.Start();

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mesh->meshPath.c_str())) {
            //throw std::runtime_error(warn + err);

            std::cout << "Warning: " << warn.c_str() << std::endl;
            std::cout << "Error: " << err.c_str() << std::endl;
            std::cout << "Failed to load" << std::endl;

            return false;
        }

        timer.Stop();

        std::cout << "File parsing time: " << timer.ElapsedMilliseconds() << "ms" << std::endl;

        std::cout << "Vertex count: " << (int)(attrib.vertices.size()) / 3 << std::endl;
        std::cout << "Normal count: " << (int)(attrib.normals.size()) / 3 << std::endl;
        std::cout << "Texcoord count: " << (int)(attrib.texcoords.size()) / 2 << std::endl;
        std::cout << "Material count: " << (int)(materials.size()) << std::endl;
        std::cout << "Shape count: " << (int)(shapes.size()) << std::endl;

        timer.Start();

        for (const auto& shape : shapes) {
            Data::SubMesh subMesh;
            std::unordered_map<Data::Vertex, uint32_t> uniqueVertices {};

            for (const auto& index : shape.mesh.indices) {
                Data::Vertex vertex {};

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

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(subMesh.vertices.size());
                    subMesh.vertices.push_back(vertex);
                }

                subMesh.indices.push_back(uniqueVertices[vertex]);
            }
            // Add the submesh
            mesh->submeshes.push_back(subMesh);
        }

        timer.Stop();

        std::cout << "Object parsing time: " << timer.ElapsedMilliseconds() << "ms" << std::endl;
    
        return true;
    }
}