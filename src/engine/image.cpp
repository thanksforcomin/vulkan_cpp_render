#include "include/engine/image.hpp"

namespace engine {
    Image::Image(VulkanContext *vulkan_context, VkImageLayout image_layout) : 
        context(vulkan_context), 
        layout(image_layout),
        image(vulkan::allocate_image(
            context->allocator,
            vulkan::extent_2d_to_3d(context->swap_chain.swap_chain_extent),
            context->swap_chain.swap_chain_image_format,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY
            )
        ),
        image_view(VK_NULL_HANDLE)
    { }

    Image::Image(VulkanContext *vulkan_context, VkImageLayout image_layout, VkExtent3D extent, VkFormat format) :
        context(vulkan_context),
        layout(image_layout),
        image(vulkan::allocate_image(
                context->allocator, 
                extent, 
                format, 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
                VMA_MEMORY_USAGE_GPU_ONLY
            )
        ),
        image_view(VK_NULL_HANDLE)
    { }
}