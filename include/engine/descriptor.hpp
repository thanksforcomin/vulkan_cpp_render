#pragma once

#include "include/engine/details.hpp"
#include "include/engine/buffers.hpp"

#include <iostream>
#include <vector>

namespace engine {
    class VulkanContext; //forward decl

    class DescriptorPool {
        private:
            VulkanContext const* context;
            std::vector<VkDescriptorPoolSize> pool_sizes;
        
        public:
            DescriptorPool(VulkanContext const* vulkan_context);
            ~DescriptorPool();

            VkDescriptorPool pool;

            void push(VkDescriptorType pool_type, uint32_t pool_size);
            void init();
    };

    class DescriptorSet {
        private:
            VulkanContext const* context;   

            std::vector<VkDescriptorSetLayoutBinding> bindings;
            
            std::vector<VkDescriptorBufferInfo> descriptor_buffers;
            std::vector<VkWriteDescriptorSet> write_descriptor_sets;

        public:
            DescriptorSet(VulkanContext *vulkan_context);
            ~DescriptorSet();

            VkDescriptorSetLayout layout;
            VkDescriptorSet descriptor_set;

            void push_layout_binding(VkDescriptorType type, VkShaderStageFlagBits shader_stage = VK_SHADER_STAGE_VERTEX_BIT, uint32_t binding_point = 0);
            void create_layout();
            void allocate(DescriptorPool &pool);
            
            template<typename T>
            void push_buffer_binding(UniformBuffer<T>& buffer, VkDescriptorType type, uint32_t binding) {
                descriptor_buffers.push_back(VkDescriptorBufferInfo{});
                VkDescriptorBufferInfo &buffer_info = descriptor_buffers.back();
                buffer_info.buffer = buffer.buffer;
                buffer_info.offset = 0;
                buffer_info.range = sizeof(T);

                write_descriptor_sets.push_back(VkWriteDescriptorSet{});
                VkWriteDescriptorSet &descriptor_write = write_descriptor_sets.back();
                descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_write.dstSet = descriptor_set;
                descriptor_write.dstBinding = binding;
                descriptor_write.descriptorCount = 1;
                descriptor_write.pBufferInfo = &buffer_info;
                descriptor_write.descriptorType = type;
            }
            //void push_texture_binding(); //TODO
            void update_buffers();
    };
}