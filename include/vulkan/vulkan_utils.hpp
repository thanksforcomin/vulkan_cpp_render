#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"

namespace vulkan {
    std::vector<const char*> require_extensions();

    bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface);

    bool device_extensions_support(VkPhysicalDevice &dev, std::vector<const char*> extensions);

    swap_chain_support_details get_swap_chain_support(VkPhysicalDevice &dev, VkSurfaceKHR &surface);
    
    queue_family_indicies find_queue_family(VkPhysicalDevice &dev, VkSurfaceKHR &surface);

    VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes);

    VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);
            
    VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats); 
}
