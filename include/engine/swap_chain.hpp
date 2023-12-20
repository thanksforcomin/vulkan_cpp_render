#pragma once

#ifndef __SWAPCHAIN_H__
#define __SWAPCHAIN_H__
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/engine/framebuffer.hpp"

namespace engine {
    class VulkanContext; // forward declaration

    class SwapChain {
        public:
            VkExtent2D swap_chain_extent;
            VkFormat swap_chain_image_format;

            VulkanContext *vulkan_context; //context to which swap chain related to

            SwapChain() = delete;
            SwapChain(VulkanContext *context);
            ~SwapChain();

            VkSwapchainKHR swap_chain;

            VkFramebuffer &query_framebuffer(uint32_t index);
            VkImageView &query_image_view(uint32_t index);
            void create_framebuffers(RenderPass &render_pass);
            uint32_t query_next_image(VkSemaphore &semop);

        private:
            //images and their views
            std::vector<VkImage> swap_chain_images;
            std::vector<VkImageView> swap_chain_image_views;
            std::vector<VkFramebuffer> swap_chain_framebuffers;

            void create_image_views();
    };
}
#endif