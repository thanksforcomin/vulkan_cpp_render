#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

namespace engine {
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
    }
}