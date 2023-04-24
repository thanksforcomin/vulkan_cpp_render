#include "include/engine/buffers.hpp"
#include "include/engine/vulkan_context.hpp"

#include <stdexcept>

namespace engine {
    //Framebuffer
    Framebuffer::Framebuffer(uint32_t count, VulkanContext *vulkan_context, VkImageView *attachment)  : context(vulkan_context) {
        VkFramebufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.renderPass = context->render_pass;
        create_info.attachmentCount = 1;
        create_info.pAttachments = attachment;
        create_info.width = context->swap_chain.swap_chain_extent.width;
        create_info.height = context->swap_chain.swap_chain_extent.height;
        create_info.layers = 1;

        if(vkCreateFramebuffer(context->device.logical, &create_info, nullptr, &data) != VK_SUCCESS)
            throw std::runtime_error("failed to create framebuffer");
    }

    Framebuffer::~Framebuffer() {
        vkDestroyFramebuffer(context->device.logical, data, nullptr);
    }

    //DescriptorSetLayout
    DescriptorSetLayout::DescriptorSetLayout(VulkanContext *vulkan_context) : context(vulkan_context), layout{} {} 

    DescriptorSetLayout::~DescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(context->device.logical, layout, nullptr);
    }

    void DescriptorSetLayout::PushBinding(
        uint32_t binding,
        VkDescriptorType descriptor_type,

        VkShaderStageFlags stage_flags,
        uint32_t count // not used so far, will be used later
    ) 
    {
        VkDescriptorSetLayoutBinding binding_info{};
        binding_info.binding = binding;
        binding_info.descriptorType = descriptor_type;
        binding_info.stageFlags = stage_flags;
        binding_info.descriptorCount = count;
        binding_info.pImmutableSamplers = nullptr; //optional

        layout_binding_descriptors.push_back(binding_info);
    }

    void DescriptorSetLayout::Build() {
        VkDescriptorSetLayoutCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        create_info.bindingCount = (uint32_t)layout_binding_descriptors.size();
        create_info.pBindings = &layout_binding_descriptors[0];

        if (vkCreateDescriptorSetLayout(context->device.logical, &create_info, nullptr, &layout) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor set layout");
    }

    //DescriptorPool
    DescriptorPool::DescriptorPool() {
        VkDescriptorPoolSize size{};
        size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        size.descriptorCount = (uint32_t)MAX_FRAMES_IN_FLIGHT;

        VkDescriptorPoolCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        create_info.poolSizeCount = 1;
        create_info.pPoolSizes = &size;
    }
}