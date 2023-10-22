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
            void init(std::vector<VkDescriptorPoolSize> pool_sizes);
            void init();
    };

    class DescriptorSetLayout {
        VulkanContext *context;

    public:
        DescriptorSetLayout(VulkanContext *vulkan_context);
        ~DescriptorSetLayout();

        void push_layout_binding(VkDescriptorType type, VkShaderStageFlags shader_stage = VK_SHADER_STAGE_VERTEX_BIT, uint32_t binding_point = 0);
        void create_layout();

        VkDescriptorSetLayout layout;

    private:
        std::vector<VkDescriptorSetLayoutBinding> bindings;
    };

    class DescriptorSet {
    private:
        VulkanContext *context;   
        VkDescriptorSetLayout *layout;

    public:
        DescriptorSet(VulkanContext *vulkan_context, VkDescriptorSetLayout *desc_layout, VkDescriptorPool *desc_pool);
        ~DescriptorSet();

        VkDescriptorSet descriptor_set;
    };
}