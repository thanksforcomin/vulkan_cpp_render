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
                                                    swap_chain_support_details swap_chain_support,
                                                    VkSurfaceKHR &surface, 
                                                    std::vector<uint32_t> queue_family_indicies,
                                                    VkSwapchainKHR old_swap_chain = VK_NULL_HANDLE);

    VkImageViewCreateInfo image_view_create_info(VkImage &image, 
                                                 VkFormat format,
                                                 VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D, 
                                                 uint32_t layers = 1, 
                                                 uint32_t levels = 0);

    VkFramebufferCreateInfo framebuffer_create_info(VkImageView *image_view, 
                                                    VkRenderPass &render_pass,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint32_t layers = 1);
}