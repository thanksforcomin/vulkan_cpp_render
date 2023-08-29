#include "include/engine/descriptor.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {

    DescriptorPool::DescriptorPool(VulkanContext *vulkan_context) : context(vulkan_context) { }

    DescriptorPool::~DescriptorPool() { vkDestroyDescriptorPool(context->device.logical, pool, nullptr); }

    void DescriptorPool::push(VkDescriptorType pool_type, uint32_t pool_size) {
        pool_sizes.push_back(VkDescriptorPoolSize {pool_type, pool_size} );
    }

    void DescriptorPool::init() {
        pool = vulkan::create_descriptor_pool(context->device.logical, pool_sizes, 100); 
    }
}

namespace engine {
    DescriptorSet::DescriptorSet(VulkanContext *vulkan_context) : context(vulkan_context) {

    }

    DescriptorSet::~DescriptorSet() {
        vkDestroyDescriptorSetLayout(context->device.logical, layout, nullptr);
    }

    void DescriptorSet::push_layout_binding(VkDescriptorType type, VkShaderStageFlagBits shader_stage, uint32_t binding_point) {
        bindings.push_back(vulkan::get_descriptor_set_layout_binding(type, shader_stage, binding_point));
    }

    void DescriptorSet::create_layout() {
        layout = vulkan::create_descriptor_set_layout(context->device.logical, bindings);
    }

    void DescriptorSet::allocate(DescriptorPool &pool) {
        descriptor_set = vulkan::allocate_descriptor_set(context->device.logical, pool.pool, layout);
    }

    void DescriptorSet::update_buffers() {
        vkUpdateDescriptorSets(context->device.logical, (uint32_t)write_descriptor_sets.size(), &write_descriptor_sets[0], 0, nullptr);
    }
}