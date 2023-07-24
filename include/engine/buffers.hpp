#pragma once

#include "include/engine/details.hpp"
#include "include/engine/vulkan_context.hpp"

#include <unordered_set>

namespace engine {
    //TODO: a TPP file for this class alone
    template<typename T>
    class UniformBuffer {
        friend class DescriptorSet;
        VulkanContext *context;

        public:
            UniformBuffer(VulkanContext *vulkan_context) :
                context(vulkan_context),
                buffer(allocate_buffer(context->allocator, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU))
            { };
            ~UniformBuffer() { };

            allocated_buffer buffer;
        private:
            uint64_t size;
    };

    class VertexBuffer {
        friend class Mesh;
        VulkanContext *context;

        public:
            VertexBuffer(VulkanContext *vulkan_context, std::vector<vertex::Vertex> &&data);
            ~VertexBuffer();

            void load(std::vector<vertex::Vertex> &&data);
            allocated_buffer buffer;
    };

    /*class TextureSampler : public Buffer {
        friend class DescriptorSet;

        public:
            TextureSa
    }*/ //TODO
}