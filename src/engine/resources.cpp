#include "include/engine/resources.hpp"
#include "include/vulkan/vulkan_utils.hpp"

namespace engine {
    Attachment::Attachment(VulkanContext *vulkan_context, Image& image) :
        context(vulkan_context), 
        id(total_id++), 
        attachment_description(vulkan::get_rendering_attachment_info(image.image_view, image.layout))
    { }

    Attachment::Attachment(VulkanContext *vulkan_context) : 
        context(vulkan_context),
        id(total_id++)
    { }

    int Attachment::total_id = 0;
}