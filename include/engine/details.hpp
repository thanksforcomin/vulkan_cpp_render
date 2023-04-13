#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <optional>

namespace engine {
    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    } // i have no idea what this code does it's just boost hasher

    struct swap_chain_support_details {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    struct QueueFamilyIndicies
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        
        bool is_complete() { return (graphics_family.has_value() && present_family.has_value()); }
    };

    struct pipeline_creation_info {
        
    };

    struct vulkan_device {
        VkPhysicalDevice physical;
        VkDevice logical;
    };

    enum class queue_families {
        GRAPHICS, PRESENT
    };

    //vertex stuff
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
                hash_combine(seed, pos);
                hash_combine(seed, normal);
                hash_combine(seed, tex_coord);
                hash_combine(seed, tangent);
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