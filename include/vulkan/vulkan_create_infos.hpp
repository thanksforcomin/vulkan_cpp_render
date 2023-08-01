#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"

#include <string>
#include <vector>
#include <set>

namespace vulkan {
    VkInstanceCreateInfo instance_create_info(VkApplicationInfo *app, 
                                              const std::vector<const char*> &val_layer, 
                                              const std::vector<const char*> &extensions);
    
    VkDeviceCreateInfo logical_device_create_info(VkPhysicalDeviceFeatures *features,
                                                const std::vector<VkDeviceQueueCreateInfo> &queue_infos,
                                                const std::vector<const char*> &extensions);

    VkSwapchainCreateInfoKHR swap_chain_create_info(GLFWwindow *window, 
                                                    uint32_t min_image_count,
                                                    swap_chain_support_details &swap_chain_support,
                                                    VkSurfaceKHR &surface, 
                                                    std::set<uint32_t> &queue_family_indicies,
                                                    VkSwapchainKHR old_swap_chain = VK_NULL_HANDLE);
}