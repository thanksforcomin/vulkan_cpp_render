#include "include/engine/descriptor.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {
    DescriptorSet::DescriptorSet(VulkanContext *vulkan_context) : context(vulkan_context) {

    }

    void DescriptorSet::push_layout_binding(VkDescriptorType type, VkShaderStageFlagBits shader_stage, uint32_t binding_point) {
        VkDescriptorSetLayoutBinding binding{};
        binding.binding = std::max(binding_point, (uint32_t)bindings.size());
        binding.descriptorCount = 1;
        binding.descriptorType = type;
        binding.stageFlags = shader_stage; //todo: do something with it
        binding.pImmutableSamplers = nullptr;

        bindings.push_back(binding);
    }

    void DescriptorSet::create_layout() {
        VkDescriptorSetLayoutCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        create_info.bindingCount = (uint32_t)bindings.size();
        create_info.pBindings = &bindings[0];
        
        if(vkCreateDescriptorSetLayout(context->device.logical, &create_info, nullptr, &layout) != VK_SUCCESS) 
            throw std::runtime_error("failed to create descriptor set layout");
        std::cout << "descriptor set layout created\n";
    }

    void DescriptorSet::allocate(DescriptorPool &pool) {
        VkDescriptorSetAllocateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        create_info.pNext = nullptr;
        create_info.descriptorSetCount = 1;
        create_info.descriptorPool = pool.pool;
        create_info.pSetLayouts = &layout;

        if(vkAllocateDescriptorSets(context->device.logical, &create_info, &descriptor_set) != VK_SUCCESS) 
            throw std::runtime_error("failed to allocate descriptor set");
        std::cout << "allocated descriptor set\n";
    }

    void DescriptorSet::update_buffers() {
        vkUpdateDescriptorSets(context->device.logical, (uint32_t)write_descriptor_sets.size(), &write_descriptor_sets[0], 0, nullptr);
    }
    /*
    template<typename T>
    void DescriptorSet::assign_uniform_buffer(UniformBuffer<T>& buffer) {
        
    }*/
}

namespace engine {
    DescriptorSet::~DescriptorSet() {
        vkDestroyDescriptorSetLayout(context->device.logical, layout, nullptr);
    }

    DescriptorPool::DescriptorPool(VulkanContext const* vulkan_context) : context(vulkan_context) { }

    DescriptorPool::~DescriptorPool() { vkDestroyDescriptorPool(context->device.logical, pool, nullptr); }

    void DescriptorPool::push(VkDescriptorType pool_type, uint32_t pool_size) {
        pool_sizes.push_back(VkDescriptorPoolSize {pool_type, pool_size} );
    }

    void DescriptorPool::init() {
        VkDescriptorPoolCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        create_info.poolSizeCount = (uint32_t)pool_sizes.size();
        create_info.pPoolSizes = &pool_sizes[0];
        create_info.maxSets = 200;
        create_info.flags = 0;

        if(vkCreateDescriptorPool(context->device.logical, &create_info, nullptr, &pool) != VK_SUCCESS) 
            throw std::runtime_error("failed to create descriptor pool");
        std::cout << "descriptor pool created\n"; 
    }
}