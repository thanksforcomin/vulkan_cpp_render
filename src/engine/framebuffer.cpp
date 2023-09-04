#include "include/engine/framebuffer.hpp"
#include "include/engine/vulkan_context.hpp"

#include <stdexcept>
#include <iostream>

namespace engine {
    Framebuffer::Framebuffer(const VulkanContext *vulkan_context, 
                             RenderPass &render_pass, 
                             VkImageView *image_attachment,
                             uint32_t width, uint32_t height, uint32_t layers) : context(vulkan_context) 
    {
        VkFramebufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.pNext = nullptr;
        create_info.renderPass = render_pass.render_pass;
        create_info.attachmentCount = 1;
        create_info.width = width;
        create_info.height = height;
        create_info.layers = 1;
        create_info.pAttachments = image_attachment;
        
        if(vkCreateFramebuffer(vulkan_context->device.logical, &create_info, nullptr, &data) != VK_SUCCESS) {
            throw std::runtime_error("error when creating framebuffer");
        }
        std::cout << "framebuffer successfully created\n";
    }

    Framebuffer::~Framebuffer()
    {
        //vkDestroyFramebuffer(context->device.logical, data, nullptr);
    }
}
