#pragma once

#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_initializers.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    class Image {
    private:
        VulkanContext *context;

    public:
        Image() = delete;
        Image(VulkanContext *vulkan_context, VkImageLayout image_layout);
        ~Image();

        VkImage image;
        VkImageView image_view;

        const VkImageLayout layout;
    };
}