#pragma once

#include "include/vulkan/vulkan_utils.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    class VertexBuffer {
    private:    
        VulkanContext *context;

    public:
        VertexBuffer() = delete;
        VertexBuffer(VulkanContext *vulkan_context, std::vector<vulkan::vertex::Vertex> *data);
        ~VertexBuffer();

        void upload(std::vector<vulkan::vertex::Vertex> *data);
        void bind(VkCommandBuffer &command_buffer, uint32_t binding = 0, uint32_t count = 1);

    private:
        vulkan::allocated_buffer buffer;
    };

    class IndexBuffer {
    private:    
        VulkanContext *context;
    
    public:
        IndexBuffer() = delete;
        IndexBuffer(VulkanContext *vulkan_context, std::vector<uint32_t> *data);
        ~IndexBuffer();

        void upload(std::vector<uint32_t> *data);
        void bind(VkCommandBuffer &command_buffer, uint32_t binding = 0, uint32_t count = 1);

    private:
        vulkan::allocated_buffer buffer;
    };

    class Buffer {
    private:    
        VulkanContext *context;

    public:
        Buffer() = delete;
        Buffer(VulkanContext* vulkan_context, size_t size, VkBufferUsageFlags usage_flags, VmaMemoryUsage memory_usage);
        ~Buffer();

        void upload(void* data, uint32_t size);

    private:
        vulkan::allocated_buffer buffer;
    };
}