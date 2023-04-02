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
            VkExtent2D swap_chain_extent;
            VkFormat swap_chain_image_format;

            //images and their views
            std::vector<VkImage> swap_chain_images;
            std::vector<VkImageView> swap_chain_image_views;

            VulkanContext *vulkan_context; //context to which swap chain related to

            VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes);
            VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities);
            VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats);
            void query_swap_chain_images();
            void create_image_views();
    };
}