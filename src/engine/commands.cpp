#include "include/engine/commands.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/renderpass.hpp"
#include "include/vulkan/vulkan_initializers.hpp"
#include "include/vulkan/vulkan_create_infos.hpp"

namespace engine {
    CommandBuffer::CommandBuffer(VulkanContext *vulkan_context, VkCommandPool command_pool, VkCommandBufferLevel lvl) :
        context(vulkan_context),
        pool(&command_pool),
        command_buffer(vulkan::allocate_command_buffer(context->device.logical, command_pool, lvl)),
        command_buffer_begin_info(vulkan::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
    {

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
        vkEndCommandBuffer(command_buffer);
        vkEndCommandBuffer(command_buffer);
    }
}

namespace engine {
    CommandDispatcher::CommandDispatcher(VulkanContext *vulkan_context, VkCommandBufferLevel lvl) : 
        context(vulkan_context), 
        command_buffer_begin_info{vulkan::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)},
        command_pool(vulkan::create_command_pool(context->device.logical, 
                                                 VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 
                                                 context->queue_family.graphics_family.value()))
    {

    }

    CommandDispatcher::CommandDispatcher(CommandDispatcher &&cmd) :
        command_buffer(cmd.command_buffer),
        command_pool(cmd.command_pool),
        command_buffer_begin_info(cmd.command_buffer_begin_info),
        context(cmd.context)
    {
        cmd.command_buffer = NULL;
        cmd.command_pool = NULL;
    }

    void CommandDispatcher::reset() {
        if(vkResetCommandBuffer(command_buffer, 0) != VK_SUCCESS)
            throw std::runtime_error("cannot reset main command buffer");
    }

    void CommandDispatcher::begin(const RenderPass& rp) {
        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
        vkCmdBeginRenderPass(command_buffer, &rp.begin_info, VK_SUBPASS_CONTENTS_INLINE);
    }
    
    void CommandDispatcher::end() {
        vkCmdEndRenderPass(command_buffer);
        vkEndCommandBuffer(command_buffer);
    }
    
    CommandDispatcher::~CommandDispatcher() {
        if(command_pool)
            vkDestroyCommandPool(context->device.logical, command_pool, nullptr);
    }
}