#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/engine/structs.hpp"

namespace engine {
    namespace vulkan {
        bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface);
        bool device_extensions_support(VkPhysicalDevice &dev);
        swap_chain_support_details get_swap_chain_support(VkPhysicalDevice &dev);
        queue_family_indicies find_queue_family(VkPhysicalDevice &dev, VkSurfaceKHR &surface);
    }
}