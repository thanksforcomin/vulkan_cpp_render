#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"

#include <vector>
#include <string>

namespace glfw {
    GLFWwindow* create_window(uint32_t height, uint32_t width);
}

namespace vulkan {
    VkSurfaceKHR create_surface(VkInstance &inst, GLFWwindow *window);

    VkInstance create_instance(std::string application_name);

    VkPhysicalDevice create_physical_device(VkInstance &inst, VkSurfaceKHR &surface);

    VkDevice create_logical_device(VkPhysicalDevice &dev, VkSurfaceKHR &surface);

    VkQueue create_queue(VkDevice &dev, uint32_t queue_family_index);
    
    VkSwapchainKHR create_swap_chain(vulkan_device &vulkan_dev, VkSurfaceKHR &surface, GLFWwindow *window);

    VkImageView create_image_view(VkDevice &dev, VkImage &image, VkFormat &format);

    VkFramebuffer create_framebuffer(VkDevice &dev, VkRenderPass &render_pass, VkImageView *image_attachment, VkExtent2D extent);
}
