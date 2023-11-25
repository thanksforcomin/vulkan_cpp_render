#include "include/engine/image.hpp"

namespace engine {
    Image::Image(VulkanContext *vulkan_context, VkImageLayout image_layout) : 
        context(vulkan_context), 
        layout(image_layout),
        image(vulkan::create_image()),
        image_view(VK_NULL_HANDLE)
    {

    }
}