#pragma once

#define boid void //i was bored

#include "include/engine/details.hpp"
//here will be vertex buffers and uniform buffers.
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

    class DescriptorSetLayout {
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
}