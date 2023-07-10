#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

//#include <vulkan/vulkan.hpp> //cpp style vulkan here
//it makes compilation slower and i use C style always anyway

#include <stdexcept>
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

    struct vulkan_device {
        VkPhysicalDevice physical;
        VkDevice logical;
    };

    enum class queue_families {
        GRAPHICS, PRESENT
    };

    //functions
    /*VkFormat find_supported_format(std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags flags, vulkan_device device){
        for(auto& format : candidates) {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(device.physical, format, &properties);

            if(tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & flags) == flags)
                return format;
            if(tiling == VK_IMAGE_TILING_OPTIMAL && (properties.linearTilingFeatures & flags) == flags)
                return format;
        }
        throw std::runtime_error("unable to find supported format");
    };*/

    inline VkAttachmentDescription default_attachment(VkFormat format) {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        return color_attachment;
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

    namespace defaults {
        constexpr VkPipelineVertexInputStateCreateInfo _vertex_input_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            nullptr, 
            0,
            1,
            &vertex::bind_description,
            4,
            &vertex::attrib_descriptions[0]
        };

        constexpr VkPipelineInputAssemblyStateCreateInfo _assembly_input_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            nullptr,
            0,
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VK_FALSE
        };

        constexpr VkPipelineRasterizationStateCreateInfo _rasterization_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            nullptr,
            0,
            VK_FALSE,
            VK_FALSE,
            VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_BACK_BIT,
            VK_FRONT_FACE_CLOCKWISE,
            VK_FALSE,
            0.0f,
            0.0f,
            0.0f,
            1.0f
        };

        constexpr VkPipelineMultisampleStateCreateInfo _multisample_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            nullptr, 
            0,
            VK_SAMPLE_COUNT_1_BIT,
            VK_FALSE,
            1.0f,
            nullptr,
            VK_FALSE,
            VK_FALSE
        }; 

        constexpr VkPipelineDepthStencilStateCreateInfo _depth_and_stencil_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            nullptr,
            0,
            VK_FALSE,
            VK_FALSE,
            VK_COMPARE_OP_LESS_OR_EQUAL,
            VK_FALSE,
            VK_FALSE,
            VkStencilOpState{},
            VkStencilOpState{},
            0.0f,
            0.0f
        };

        constexpr VkPipelineColorBlendAttachmentState _color_blend_attachment_state {
            VK_TRUE,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_ADD,
            VK_BLEND_FACTOR_ONE,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD, 
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
        };

        constexpr VkPipelineColorBlendStateCreateInfo _color_blend_attachment_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            nullptr, 
            0,
            VK_FALSE,
            VK_LOGIC_OP_COPY,
            1,
            &_color_blend_attachment_state,
            {0.0f, 0.0f, 0.0f, 0.0f}
        };

        constexpr VkDynamicState _dynamic_states[2] { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

        constexpr VkPipelineDynamicStateCreateInfo _dynamic_state_create_info {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            nullptr,
            0,
            2,
            &_dynamic_states[0]
        };

        inline VkPipelineViewportStateCreateInfo _get_viewport_state_create_info(const VkViewport *viewport, const VkRect2D *scissors) {
            return VkPipelineViewportStateCreateInfo {
                VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                nullptr, 
                0, 
                1, 
                viewport,
                1,
                scissors
            };
        };
    }
}