#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "include/utils/utils.hpp"

namespace vulkan {
    namespace vertex {
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 tex_coord;
            glm::vec3 tangent;

            bool operator==(const Vertex &other) const noexcept {
                return pos == other.pos &&
                    normal == other.normal &&
                    tex_coord == other.tex_coord &&
                    tangent == other.tangent;
            }

            size_t hash() const { //hashes the whole vertex
                size_t seed = 0;
                utils::hash_combine(seed, pos);
                utils::hash_combine(seed, normal);
                utils::hash_combine(seed, tex_coord);
                utils::hash_combine(seed, tangent);
                return seed;
            }
        };

            //these two were supposed to be constexpr
        constexpr const VkVertexInputBindingDescription bind_description = { //the binding description for basic vertex
            0, // binding
            sizeof(Vertex), //stride
            VK_VERTEX_INPUT_RATE_VERTEX //input rate
        };

        constexpr const VkVertexInputAttributeDescription attrib_descriptions[4] = { //the attribute description for basic vertex
            {
                0, //location
                0, //binding
                VK_FORMAT_R32G32B32_SFLOAT, //format
                offsetof(Vertex, pos) //offset
            },
            {
                1, //location
                0, //binding
                VK_FORMAT_R32G32B32_SFLOAT, //format
                offsetof(Vertex, normal) //offset
            },
            {
                2, //location
                0, //binding
                VK_FORMAT_R32G32_SFLOAT, //format
                offsetof(Vertex, tex_coord) //offset
            },
            {
                3, //location
                0, //binding
                VK_FORMAT_R32G32B32_SFLOAT, //format
                offsetof(Vertex, tangent) //offset
            }
        };
    }
}