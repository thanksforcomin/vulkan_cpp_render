#pragma once

#ifndef __BUFFERS_H__
#define __BUFFERS_H__
#include "include/engine/renderpass.hpp"

namespace engine {
    class VulkanContext;

    class Framebuffer {
        public:
            Framebuffer(const VulkanContext *vulkan_context, 
                        RenderPass &render_pass,
                        VkImageView *image_attachment, 
                        uint32_t width, uint32_t height, uint32_t layers = 1);
            Framebuffer(const Framebuffer &fb) = default;
            Framebuffer(Framebuffer &&fb) = default;
            ~Framebuffer();

            VkFramebuffer data;

        private:
            const VulkanContext *context;
    };
}
#endif