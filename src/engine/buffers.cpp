#include "include/engine/buffers.hpp"
#include "include/engine/vulkan_context.hpp"

#include <stdexcept>

namespace engine {
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
}