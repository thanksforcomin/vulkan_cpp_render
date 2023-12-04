#include "include/engine/dynamic_rendering.hpp"

#include "include/vulkan/vulkan_utils.hpp"

namespace engine {
    DynamicRenderPass::DynamicRenderPass(VulkanContext *vulkan_context) :
        context(vulkan_context), id(next_id++)
    { 
        rendering_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
            .renderArea = {
                .offset = {0, 0},
                .extent = vulkan_context->swap_chain.swap_chain_extent
            },
            .layerCount = 1,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment,
            .pDepthAttachment = &depth_attachment,
            .pStencilAttachment = &stencil_attachment
        };
    };

    DynamicRenderPass::~DynamicRenderPass() { };

    void DynamicRenderPass::begin(VkCommandBuffer& cmd_buffer) {
        vkCmdBeginRendering(cmd_buffer, &rendering_info);
    };

    void DynamicRenderPass::end(VkCommandBuffer& cmd_buffer) {
        vkCmdEndRendering(cmd_buffer);
    };

    void DynamicRenderPass::set_color_attachment(Attachment& attachment) {
        color_attachment = attachment.attachment_description;
    }

    void DynamicRenderPass::set_depth_attachment(Attachment& attachment, bool write_enabled) {
        depth_attachment = attachment.attachment_description;
        depth_attachment.loadOp = write_enabled ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment.storeOp = write_enabled ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }

    void DynamicRenderPass::set_stencil_attachment(Attachment& attachment) {
        stencil_attachment = attachment.attachment_description;
    }

    int DynamicRenderPass::next_id = 0;
}