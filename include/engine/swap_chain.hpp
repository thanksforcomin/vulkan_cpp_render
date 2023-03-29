#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/engine/details.hpp"
#include "include/engine/window.hpp"

namespace engine {
    class VulkanContext; // forward declaration

    class SwapChain {
        public:
            SwapChain(VulkanContext *context);
            ~SwapChain();

        private:
            VkSwapchainKHR swap_chain;
            std::vector<VkImage> swap_chain_images;

            const VulkanContext *vulkan_context; //context to which swap chain related to

            VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes);
            VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities);
            VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats);
            uint32_t query_swap_chain_images();
    };
}