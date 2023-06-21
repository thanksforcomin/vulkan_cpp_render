#pragma once

#include "include/engine/details.hpp"
#include "include/engine/buffers.hpp"

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

        public:
            DescriptorSet(VulkanContext *vulkan_context);
            ~DescriptorSet();

            VkDescriptorSetLayout layout;
            VkDescriptorSet descriptor_set;

            void push(VkDescriptorType type, VkShaderStageFlagBits shader_stage = VK_SHADER_STAGE_VERTEX_BIT, uint32_t binding_point = 0);
            void create_layout();
            void allocate(DescriptorPool &pool);
    };
}