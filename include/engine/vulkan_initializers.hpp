#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

namespace engine {
    namespace vulkan {
        std::vector<const char*> get_required_extensions();
        VkSurfaceKHR create_surface(VkInstance &inst, GLFWwindow *window);
        VkInstance create_instance(std::string application_name);
        VkPhysicalDevice create_physical_device(VkInstance &inst);
        VkDevice create_logical_device(VkPhysicalDevice &dev);
    }
}