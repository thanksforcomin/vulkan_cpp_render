#pragma once

#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_initializers.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/upload_context.hpp"

namespace engine {
    class Image {
        VulkanContext *context;

    private:
        const VkExtent3D extent;

    public:
        Image() = delete;
        Image(VulkanContext *vulkan_context, VkImageLayout image_layout);
        Image(VulkanContext *vulkan_context, VkImageLayout image_layout, VkExtent3D img_extent, VkFormat format);
        ~Image();

        vulkan::allocated_image image;
        VkImageView image_view;

        void create_image(vulkan::allocated_buffer&& buffer, UploadContext& upload_context);

        const VkImageLayout layout;
    };
}