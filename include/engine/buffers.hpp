#pragma once

#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vertex.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    class VertexBuffer {
    private:    
        VulkanContext *context;

    public:
        VertexBuffer() = delete;
        VertexBuffer(VulkanContext *vulkan_context);
        VertexBuffer(VulkanContext *vulkan_context, std::vector<vulkan::vertex::Vertex> *data);
        ~VertexBuffer();

        void upload(std::vector<vulkan::vertex::Vertex> *data);
        void bind(VkCommandBuffer &command_buffer, uint32_t binding = 0, uint32_t count = 1);

    private:
        vulkan::allocated_buffer buffer;
    };

    class UniformBuffer {
    private:    
        VulkanContext *context;

    public:
        UniformBuffer() = delete;

        template<typename T>
        UniformBuffer(VulkanContext *vulkan_context) :
            context(vulkan_context),
            buffer(
                vulkan::allocate_buffer(
                    context->allocator,
                    sizeof(T),
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VMA_MEMORY_USAGE_CPU_TO_GPU
                )
            )
        { }
        ~UniformBuffer();

        vulkan::allocated_buffer buffer;
    };

    class Image {
    private:
        VulkanContext *context;

    public:
        Image() = delete;
        Image(VulkanContext *vulkan_context, std::string filepath);
        ~Image();
    };
}