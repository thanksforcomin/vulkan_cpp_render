#include "include/engine/buffers.hpp"

//VERTEX BUFFER
namespace engine {
    VertexBuffer::VertexBuffer(VulkanContext *vulkan_context, std::vector<vulkan::vertex::Vertex> *data) :
        context(vulkan_context),
        buffer(
            vulkan::memory::allocate_buffer(
                context->allocator, 
                sizeof(vulkan::vertex::Vertex) * data->size(), 
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                VMA_MEMORY_USAGE_CPU_TO_GPU
            )
        )
    {
        upload(data);
    }

    VertexBuffer::~VertexBuffer() {
        vmaDestroyBuffer(context->allocator, buffer.buffer, buffer.allocation);
    }

    void VertexBuffer::upload(std::vector<vulkan::vertex::Vertex> *data) {
        vulkan::memory::upload_to_buffer(buffer, data->data(), data->size() * sizeof(vulkan::vertex::Vertex));
    }    

    void VertexBuffer::bind(VkCommandBuffer &command_buffer, uint32_t binding, uint32_t count) {
        vkCmdBindVertexBuffers(command_buffer, binding, count, &buffer.buffer, nullptr);
    }
}

//INDEX_BUFFER
namespace engine {
    IndexBuffer::IndexBuffer(VulkanContext *vulkan_context, std::vector<uint32_t> *data) :
        context(vulkan_context),
        buffer(
            vulkan::memory::allocate_buffer(
                context->allocator, 
                sizeof(uint32_t) * data->size(), 
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                VMA_MEMORY_USAGE_CPU_TO_GPU
            )
        )
    {
        upload(data);
    }

    IndexBuffer::~IndexBuffer() {
        vmaDestroyBuffer(context->allocator, buffer.buffer, buffer.allocation);
    }

    void IndexBuffer::upload(std::vector<uint32_t> *data) {
        vulkan::memory::upload_to_buffer(buffer, data->data(), data->size() * sizeof(uint32_t));
    }

    void IndexBuffer::bind(VkCommandBuffer &command_buffer, uint32_t binding, uint32_t count) {
        vkCmdBindIndexBuffer(command_buffer, buffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    }
}

//GENERAL BUFFER
namespace engine {
    Buffer::Buffer(VulkanContext* vulkan_context, size_t size, VkBufferUsageFlags usage_flags, VmaMemoryUsage memory_usage) :
        context(vulkan_context),
        buffer(
            vulkan::memory::allocate_buffer(
                context->allocator, 
                size, 
                usage_flags, 
                memory_usage
            )
        )
    { }

    Buffer::~Buffer() {
        vmaDestroyBuffer(context->allocator, buffer.buffer, buffer.allocation);
    }

    void Buffer::upload(void* data, uint32_t size) {
        vulkan::memory::upload_to_buffer(buffer, data, size);
    }
}