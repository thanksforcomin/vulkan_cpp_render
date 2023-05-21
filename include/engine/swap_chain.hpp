#pragma once

#ifndef __SWAPCHAIN_H__
#define __SWAPCHAIN_H__
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/engine/details.hpp"
#include "include/engine/window.hpp"
#include "include/engine/framebuffer.hpp"

namespace engine {
    class VulkanContext; // forward declaration

    class SwapChain {
        public:
            VkExtent2D swap_chain_extent;
            VkFormat swap_chain_image_format;

            SwapChain(VulkanContext *context);
            ~SwapChain();

            VkSwapchainKHR swap_chain;

            Framebuffer &query_framebuffer(uint32_t index);
            void create_framebuffers(RenderPass &render_pass);
            uint32_t query_next_image(VkSemaphore &semop);

        private:
            //images and their views
            std::vector<VkImage> swap_chain_images;
            std::vector<VkImageView> swap_chain_image_views;
            std::vector<Framebuffer> swap_chain_framebuffers;

            const VulkanContext *vulkan_context; //context to which swap chain related to

            VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes);
            VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities);
            VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats);
            void query_swap_chain_images();
            void create_image_views();
    };
}
#endif