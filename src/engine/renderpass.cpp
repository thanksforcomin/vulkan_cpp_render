#include "include/engine/renderpass.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {
    //RenderPass
    RenderPass::RenderPass(VulkanContext *vulkan_context) :
        context(vulkan_context),
        initialized(false)
    { 
        clear_values.push_back({ .color = {0.0f, 0.0f, 1.0f, 1.0f} });
    }

    RenderPass::~RenderPass() {
        if(initialized)
            vkDestroyRenderPass(context->device.logical, render_pass, nullptr);
        std::cout << "destroyed render pass" << "\n";
    }

    //TODO: delet
    void RenderPass::init_default() { //dumb on purpose
        VkAttachmentDescription color_attachment{};
        color_attachment.format = context->swap_chain.swap_chain_image_format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        VkRenderPassCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        create_info.attachmentCount = 1;
        create_info.pAttachments = &color_attachment;
        create_info.subpassCount = 1;
        create_info.pSubpasses = &subpass;

        if(vkCreateRenderPass(context->device.logical, &create_info, nullptr, &render_pass) != VK_SUCCESS)
            throw std::runtime_error("unable to create default render pass");
        std::cout << "created render pass\n";

        begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin_info.pNext = nullptr;
        begin_info.renderPass = render_pass;
        begin_info.framebuffer = nullptr; // we will set it later, that's why it's public
        begin_info.renderArea.offset = {0, 0};
        begin_info.renderArea.extent = context->swap_chain.swap_chain_extent;
        begin_info.clearValueCount = clear_values.size();
        begin_info.pClearValues = clear_values.data();
    }
}