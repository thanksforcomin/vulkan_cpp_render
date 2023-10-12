#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_initializers.hpp"

#include <iostream>
#include <vector>

namespace engine {
    class VulkanContext; //forward decl

    class DescriptorPool {
        private:
            VulkanContext *context;
            std::vector<VkDescriptorPoolSize> pool_sizes;
        
        public:
            DescriptorPool(VulkanContext *vulkan_context);
            ~DescriptorPool();

            VkDescriptorPool pool;

            void push(VkDescriptorType pool_type, uint32_t pool_size);
            void init();
    };

    class DescriptorSet {
        private:
            VulkanContext *context;   

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
            void push_buffer_binding(vulkan::allocated_buffer& buffer, VkDescriptorType type, uint32_t binding);
            //void push_texture_binding(); //TODO
            void update_buffers();
    };
}