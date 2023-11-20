#include "include/engine/resources.hpp"
#include "include/vulkan/vulkan_utils.hpp"

namespace engine {
    Attachment::Attachment(VulkanContext *vulkan_context, Image& image) :
        context(vulkan_context), id(total_id++)
    {
        attachment_description = vulkan::get_attachment_description(image);
    }

    int Attachment::total_id = 0;
}