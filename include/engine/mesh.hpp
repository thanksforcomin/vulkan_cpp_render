#pragma once

#include "include/engine/details.hpp"
#include "include/engine/buffers.hpp"

namespace engine {
    class VulkanContext; //forward decl

    class Mesh {
        private:
            VulkanContext *context;

        public:
            Mesh(const std::string& filepath);
            Mesh(const std::vector<vertex::Vertex> &data);
            ~Mesh();

            void draw(); //TODO
    };
}