#include "include/engine/dynamic_rendering.hpp"

namespace engine {
    DynamicRenderPass::DynamicRenderPass(VulkanContext *vulkan_context) :
        context(vulkan_context), id(next_id++)
    { };

    DynamicRenderPass::~DynamicRenderPass() { };

    void DynamicRenderPass::set_color_attachment(Attachment& attachment) {
        color_attachment = attachment.attachment_description;
    }

    int DynamicRenderPass::next_id = 0;
}