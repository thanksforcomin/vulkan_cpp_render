#pragma once

#include "include/engine/details.hpp"

#include "vk_mem_alloc.h"

#include <unordered_set>

namespace engine {
    class VulkanContext; //forward decl

    class Buffer {
        private:
            VulkanContext const* context;

        public:
            Buffer(VulkanContext *vulkan_context, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage mem_usage);
            ~Buffer();

            VkBuffer buffer;
            VmaAllocation allocation;
    };

    template<typename T>
    class UniformBuffer : public Buffer {
        friend class DescriptorSet;

        public:
            UniformBuffer(VulkanContext *vulkan_context) : 
                Buffer(vulkan_context, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU) 
            { }
            ~UniformBuffer() { };
        private:
            uint64_t size;
    };

    class VertexBuffer : public Buffer {
        friend class Mesh;

        public:
            VertexBuffer(VulkanContext *context, std::vector<vertex::Vertex> &data);
            ~VertexBuffer();
    };

    /*class TextureSampler : public Buffer {
        friend class DescriptorSet;

        public:
            TextureSa
    }*/ //TODO
}