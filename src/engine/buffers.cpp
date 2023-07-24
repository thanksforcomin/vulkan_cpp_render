#include "include/engine/buffers.hpp"
#include "include/engine/vulkan_context.hpp"

#include <cstring>
#include <iostream>

#define VMA_IMPLEMENTATION

namespace engine {
    VertexBuffer::VertexBuffer(VulkanContext *vulkan_context, std::vector<vertex::Vertex> &&data) : 
        context(vulkan_context),
        buffer(allocate_buffer(context->allocator, data.size() * sizeof(vertex::Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU))
    {
        load(std::move(data));
    }

    VertexBuffer::~VertexBuffer() { }

    void VertexBuffer::load(std::vector<vertex::Vertex> &&data) {
        void *buf;
        vmaMapMemory(context->allocator, buffer.allocation, &buf);
        std::memcpy(buf, &data[0], data.size() * sizeof(vertex::Vertex));
        vmaUnmapMemory(context->allocator, buffer.allocation);
    }
}