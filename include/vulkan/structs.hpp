#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"

#include <optional>
#include <functional>
#include <vector>

namespace vulkan {
    struct queue_family_indicies {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        
        bool is_complete() { return (graphics_family.has_value() && present_family.has_value()); }
    };

    struct vulkan_device {
        VkPhysicalDevice physical;
        VkDevice logical;
    };  

    struct swap_chain_support_details {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    struct allocated_buffer {
        VkBuffer buffer;
        VmaAllocation allocation;
    };

    struct pipeline_builder {
        VkGraphicsPipelineCreateInfo create_info;

        std::vector<std::function<void()>> destructors;

        pipeline_builder& push_input_assembly(VkPipelineInputAssemblyStateCreateInfo info) {
            create_info.pInputAssemblyState = new VkPipelineInputAssemblyStateCreateInfo(info);
            destructors.push_back([=]{ delete create_info.pInputAssemblyState; });
        }
        pipeline_builder& push_vertex_assembly(VkPipelineVertexInputStateCreateInfo info) {
            create_info.pVertexInputState = new VkPipelineVertexInputStateCreateInfo(info);
            destructors.push_back([=]{ delete create_info.pVertexInputState; });
        }

        void init() {
            //code here

            for(auto& i : destructors) {
                i();
            }
        }
    };
}