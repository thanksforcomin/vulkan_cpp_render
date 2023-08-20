#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "tiny_obj_loader.h"
#include "vk_mem_alloc.h"
#include "stb_image.h"

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

    //functions

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

    namespace uniform {
        struct camera_data {
            glm::mat4 view;
            glm::mat4 proj;
            glm::mat4 viewproj;
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

        constexpr VkDynamicState _dynamic_states[2] { 
            VK_DYNAMIC_STATE_VIEWPORT, 
            VK_DYNAMIC_STATE_SCISSOR 
        };

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

        inline VkImageCreateInfo _get_image_create_info(VkFormat format, VkImageUsageFlags flags, VkExtent3D extent, uint32_t mip_levels = 1) {
            return VkImageCreateInfo {
                VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                nullptr, 
                0,
                VK_IMAGE_TYPE_2D,
                format,
                extent, 
                mip_levels,
                1,
                VK_SAMPLE_COUNT_1_BIT,
                VK_IMAGE_TILING_OPTIMAL,
                flags, 
                VK_SHARING_MODE_EXCLUSIVE,
                0, 
                nullptr,
                VK_IMAGE_LAYOUT_PREINITIALIZED
            };
        };
        
        inline VkImageViewCreateInfo _get_image_view_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspect_flags, uint32_t mip_levels = 1) {
            return VkImageViewCreateInfo {
                VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                nullptr, 
                0,
                image, 
                VK_IMAGE_VIEW_TYPE_2D,
                format,
                VkComponentMapping {
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B,
                    VK_COMPONENT_SWIZZLE_A
                },
                VkImageSubresourceRange {
                    VK_IMAGE_ASPECT_COLOR_BIT,
                    0,
                    mip_levels, 
                    0, 
                    1
                }
            };   
        };
    }

    struct allocated_buffer {
        VkBuffer buffer;
        VmaAllocation allocation;
    };

    static allocated_buffer allocate_buffer(VmaAllocator &allocator, size_t size, VkBufferUsageFlags flags, VmaMemoryUsage usage) {
        allocated_buffer buffer;

        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.pNext = nullptr;
        create_info.size = size;
        create_info.usage = flags;

        VmaAllocationCreateInfo allocation_info{};
        allocation_info.usage = usage;

        vmaCreateBuffer(allocator, &create_info, &allocation_info, &buffer.buffer, &buffer.allocation, nullptr);
        return buffer;
    };
}