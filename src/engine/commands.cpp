#include "include/engine/commands.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/renderpass.hpp"

#include "include/vulkan/vulkan_utils.hpp"

namespace engine {
    CommandBuffer::CommandBuffer(VulkanContext *vulkan_context, VkCommandPool command_pool, VkCommandBufferLevel lvl) :
        context(vulkan_context),
        pool(&command_pool),
        command_buffer(vulkan::allocate_command_buffer(context->device.logical, command_pool, lvl)),
        command_buffer_begin_info(vulkan::get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
    { }

    CommandBuffer::CommandBuffer(CommandBuffer &&buff) :
        context(buff.context),
        pool(buff.pool),
        command_buffer(buff.command_buffer),
        command_buffer_begin_info(buff.command_buffer_begin_info) 
    { 
        buff.pool = nullptr;
        buff.command_buffer = VK_NULL_HANDLE;
    }
    
    CommandBuffer::~CommandBuffer() {
        vkFreeCommandBuffers(context->device.logical, *pool, 1, &command_buffer);
    }

    void CommandBuffer::reset() {
        if(vkResetCommandBuffer(command_buffer, 0) != VK_SUCCESS)
            throw std::runtime_error("cannot reset main command buffer");
    }

    void CommandBuffer::begin(VkRenderPassBeginInfo &render_pass_begin_info) {
        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
        vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, 
                            (command_buffer_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY
                            ? VK_SUBPASS_CONTENTS_INLINE : VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS)
        );
    }

    void CommandBuffer::end() {
        vkCmdEndRenderPass(command_buffer);
        vkEndCommandBuffer(command_buffer);
    }

    CommandPool::CommandPool(VulkanContext *vulkan_context, VkCommandPoolCreateFlags flags) :
        context(vulkan_context),
        command_pool(vulkan::create_command_pool(context->device.logical, flags, context->queue_family.graphics_family.value()))
    { }

    CommandPool::CommandPool(CommandPool &&pool) :
        context(pool.context),
        command_pool(pool.command_pool)
    {
        pool.command_pool = VK_NULL_HANDLE;
    }

    CommandPool::~CommandPool() {
        vkDestroyCommandPool(context->device.logical, command_pool, nullptr);
    }
}