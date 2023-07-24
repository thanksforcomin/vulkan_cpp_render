#include "include/engine/mesh.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    Mesh::Mesh(VulkanContext *vulkan_context, const std::string &filepath) : 
        VBO(vulkan_context, load_from_file(filepath))
    {

    }

    Mesh::Mesh(VulkanContext *vulkan_context, std::vector<vertex::Vertex> &data) : 
        VBO(vulkan_context, std::move(data))
    {

    }

    Mesh::Mesh(const Mesh &other) : VBO(other.VBO) {

    }

    Mesh::~Mesh() {

    }

    std::vector<vertex::Vertex> load_from_file(std::string filepath) {
        tinyobj::attrib_t attrib;
    }
}