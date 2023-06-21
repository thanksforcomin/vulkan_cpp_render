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
    
    class VertexBuffer : public Buffer {
        public:
            VertexBuffer(VulkanContext *context, const std::vector<vertex::Vertex>& data);
    };
}