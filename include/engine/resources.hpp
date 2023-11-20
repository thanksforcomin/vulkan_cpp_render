#pragma once

#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_initializers.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/buffers.hpp"

namespace engine {
    class Attachment {
        VulkanContext *context;

        //prob a bad idea
        //we assign a new Id to each render pass, and store the next ID in a static variable
        static int total_id;
    public:
        Attachment(VulkanContext *vulkan_context, Image& image);
        Attachment(VulkanContext *vulkan_context);
        ~Attachment();

        VkRenderingAttachmentInfoKHR attachment_description;

    public:
        const int id;
    };
}