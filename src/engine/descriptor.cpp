#include "include/engine/descriptor.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {

    DescriptorPool::DescriptorPool(VulkanContext *vulkan_context) : context(vulkan_context) { }

    DescriptorPool::~DescriptorPool() { vkDestroyDescriptorPool(context->device.logical, pool, nullptr); }

    void DescriptorPool::push(VkDescriptorType pool_type, uint32_t pool_size) {
        pool_sizes.push_back(VkDescriptorPoolSize {pool_type, pool_size} );
    }

    void DescriptorPool::init(std::vector<VkDescriptorPoolSize> pool_sizes) {
        pool = vulkan::create_descriptor_pool(context->device.logical, pool_sizes, 100);
    }

    void DescriptorPool::init() {
        pool = vulkan::create_descriptor_pool(context->device.logical, pool_sizes, 100); 
    }
}

namespace engine {
    DescriptorSetLayout::DescriptorSetLayout(VulkanContext *vulkan_context) : context(vulkan_context) { }

    DescriptorSetLayout::~DescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(context->device.logical, layout, nullptr);
    }

    void DescriptorSetLayout::push_layout_binding(VkDescriptorType type, VkShaderStageFlagBits shader_stage, uint32_t binding_point) {
        bindings.push_back(vulkan::get_descriptor_set_layout_binding(type, shader_stage, binding_point));
    }

    void DescriptorSetLayout::create_layout() {
        layout = vulkan::create_descriptor_set_layout(context->device.logical, bindings);
    }
}

namespace engine {
    DescriptorSet::DescriptorSet(VulkanContext *vulkan_context, VkDescriptorSetLayout *desc_layout, VkDescriptorPool *desc_pool) : 
        context(vulkan_context),
        layout(desc_layout),
        descriptor_set(vulkan::allocate_descriptor_set(context->device.logical, *desc_pool, *layout))
    {

    }

    DescriptorSet::~DescriptorSet() { }
}