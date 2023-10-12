#include "include/engine/buffers.hpp"

//VERTEX BUFFER
namespace engine {
    VertexBuffer::VertexBuffer(VulkanContext *vulkan_context, std::vector<vulkan::vertex::Vertex> *data) :
        context(vulkan_context),
        buffer(
            vulkan::allocate_buffer(
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
        upload_to_buffer(buffer, data->data(), data->size());
    }    

    void VertexBuffer::bind(VkCommandBuffer &command_buffer, uint32_t binding, uint32_t count) {
        vkCmdBindVertexBuffers(command_buffer, binding, count, &buffer.buffer, nullptr);
    }
}

//UNIFORM BUFFER
namespace engine {

}