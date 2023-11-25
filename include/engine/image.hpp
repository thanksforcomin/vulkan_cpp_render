#pragma once

#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_initializers.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    class Image {
        VulkanContext *context;

    public:
        Image() = delete;
        Image(VulkanContext *vulkan_context, VkImageLayout image_layout);
        Image(VulkanContext *vulkan_context, VkImageLayout image_layout, VkExtent3D extent, VkFormat format);
        ~Image();

        vulkan::allocated_image image;
        VkImageView image_view;

        void load_image(const std::string &path);

        const VkImageLayout layout;
    };
}