#include "include/engine/renderpass.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {
    //RenderPass
    RenderPass::RenderPass(VulkanContext *vulkan_context, const uint32_t clear_val_count, VkClearValue *clear_val) :
        context(vulkan_context),
        clear_value_count(clear_val_count),
        clear_value(clear_val),
        initialized(false)
    { }

    RenderPass::~RenderPass() {
        if(initialized)
            vkDestroyRenderPass(context->device.logical, data, nullptr);
        std::cout << "destroyed render pass" << "\n";
    }

    void RenderPass::init(const uint32_t attachments_count,
                          VkAttachmentDescription *attachments,
                          const uint32_t subpass_count,
                          VkSubpassDescription *subpasses, 
                          const uint32_t dependencies_count,
                          VkSubpassDependency *dependencies) 
    {
        VkRenderPassCreateInfo create_info{};
        create_info.attachmentCount = attachments_count;
        create_info.pAttachments = attachments;
        create_info.subpassCount = subpass_count;
        create_info.pSubpasses = subpasses;
        create_info.dependencyCount = dependencies_count;
        create_info.pDependencies = dependencies;

        if(vkCreateRenderPass(context->device.logical, &create_info, nullptr, &data) != VK_SUCCESS)
            throw std::runtime_error("unable to create render pass");

        begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin_info.pNext = nullptr;
        begin_info.renderPass = data;
        begin_info.framebuffer = nullptr; // we will set it later, that's why it's public
        begin_info.renderArea.offset = {0, 0};
        begin_info.renderArea.extent = context->swap_chain.swap_chain_extent;
        begin_info.clearValueCount = clear_value_count;
        begin_info.pClearValues = clear_value;
    }

    void RenderPass::init_default() { //dumb on purpose
        VkAttachmentDescription color_attachment{};
        color_attachment.format = context->swap_chain.swap_chain_image_format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        VkAttachmentReference color_attachment_reference{};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        VkRenderPassCreateInfo create_info{};
        create_info.attachmentCount = 1;
        create_info.pAttachments = &color_attachment;
        create_info.subpassCount = 1;
        create_info.pSubpasses = &subpass;

        if(vkCreateRenderPass(context->device.logical, &create_info, nullptr, &data) != VK_SUCCESS)
            throw std::runtime_error("unable to create default render pass");
        std::cout << "created render pass\n";
    }
}