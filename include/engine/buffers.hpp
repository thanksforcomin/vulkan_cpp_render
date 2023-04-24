#pragma once

#define boid void

#include "include/engine/details.hpp"
//here will be vertex buffers , framebuffers, uniform buffers and stuff related to them specifically 
//the reason i placed all them here is because i find them kinda same.
//and the index buffer will be here too

namespace engine {
    class VulkanContext;

    class VertexBuffer {
    };
    class UniformBuffer {
        public:
            UniformBuffer();
            ~UniformBuffer();

            //should have:
            //description builder,
            //the descriptions themselves,
            //a way to write to them

    };
    class IndexBuffer {
    };

    class Framebuffer {
        public:
            Framebuffer(uint32_t count, VulkanContext *vulkan_context, VkImageView *attachment = nullptr);
            ~Framebuffer();

            VkFramebuffer data;
        private:
            VulkanContext *context;
    };

    class DescriptorSetLayout
    {
        public:
            DescriptorSetLayout(VulkanContext *vulkan_context);
            ~DescriptorSetLayout();
            void PushBinding(
                uint32_t binding,
                VkDescriptorType descriptor_type,
                VkShaderStageFlags stage_flags,
                uint32_t count = 1 // not used so far, will be used later
            );
            boid Build();
        private:
            std::vector<VkDescriptorSetLayoutBinding> layout_binding_descriptors;
            VkDescriptorSetLayout layout;
            VulkanContext *context;
    };

    class DescriptorPool {
        public:
            DescriptorPool();
            ~DescriptorPool();
        private:
            VkDescriptorPool pool;
    };
}