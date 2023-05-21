#include "include/engine/commands.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    CommandDispatcher::CommandDispatcher(const VulkanContext *vulkan_context, VkCommandBufferLevel lvl) : 
        context(vulkan_context), command_buffer_begin_info{} 
    {
        VkCommandPoolCreateInfo command_pool_create_info{};
        command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.pNext = nullptr;
        command_pool_create_info.queueFamilyIndex = context->find_queue_family(context->device.physical).graphics_family.value();
        if(vkCreateCommandPool(context->device.logical, &command_pool_create_info, nullptr, &command_pool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool");

        VkCommandBufferAllocateInfo command_buffer_alloc_info{};
        command_buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_alloc_info.pNext = nullptr;
        command_buffer_alloc_info.commandPool = command_pool;
        command_buffer_alloc_info.commandBufferCount = 1;
        command_buffer_alloc_info.level = lvl; //this is the buffer that we will submit as secondary to the main frame buffer

        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        command_buffer_begin_info.pInheritanceInfo = nullptr;
        command_buffer_begin_info.pNext = nullptr;
        if (vkAllocateCommandBuffers(context->device.logical, &command_buffer_alloc_info, &command_buffer) != VK_SUCCESS)
            throw std::runtime_error("cannot allocate main command buffer");
    }
    
    void CommandDispatcher::reset() {
        if(vkResetCommandBuffer(command_buffer, 0) != VK_SUCCESS)
            throw std::runtime_error("cannot reset main command buffer");
    }

    CommandDispatcher::~CommandDispatcher() {
        vkDestroyCommandPool(context->device.logical, command_pool, nullptr);
    }
}