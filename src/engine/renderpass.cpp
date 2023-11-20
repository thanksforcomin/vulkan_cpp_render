#include "include/engine/renderpass.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/vulkan/vulkan_utils.hpp"

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
        VkAttachmentDescription color_attachment = vulkan::get_color_attachment(context->swap_chain.swap_chain_image_format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        VkAttachmentReference color_attachment_reference{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        VkSubpassDescription subpass{
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_reference
        };

        render_pass = vulkan::create_render_pass(context->device.logical, {color_attachment}, {subpass}, {});

        begin_info = vulkan::get_render_pass_begin_info(render_pass, { {0, 0}, context->swap_chain.swap_chain_extent }, clear_values.data(), clear_values.size());
    }
}

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