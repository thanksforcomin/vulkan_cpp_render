#pragma once

#include "include/engine/details.hpp"
#include "include/engine/buffers.hpp"

namespace engine {
    class VulkanContext; //forward decl

    class Mesh {
        private:
            VulkanContext *context;

        public:
            Mesh(VulkanContext *vulkan_context, const std::string& filepath);
            Mesh(VulkanContext *vulkan_context, std::vector<vertex::Vertex> &data);
            Mesh(const Mesh& other);
            ~Mesh();

            void draw(); //TODO
        private:
            std::vector<vertex::Vertex> load_from_file(std::string filepath);

            VertexBuffer VBO;
    };

    class Model {
        private:
            VulkanContext *context;

        public:
            Model(VulkanContext *vulkan_context, std::string filepath);
            ~Model();

        private:
            std::vector<Mesh> meshes;
    };
}