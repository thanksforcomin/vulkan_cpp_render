#include "include/engine/buffers.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace engine {
    Buffer::Buffer(VulkanContext *vulkan_context, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage mem_usage) : context(vulkan_context) {
        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.pNext = nullptr;
        create_info.size = size;
        create_info.usage = usage;

        VmaAllocationCreateInfo alloc_info{};
        alloc_info.usage = mem_usage;
        if(vmaCreateBuffer(context->allocator, 
                           &create_info, 
                           &alloc_info, 
                           &buffer, 
                           &allocation, 
                           nullptr) 
                           != VK_SUCCESS) 
            throw std::runtime_error("unable to create buffer");
    }

    Buffer::~Buffer() {
        vmaDestroyBuffer(context->allocator, buffer, allocation);
    }
}