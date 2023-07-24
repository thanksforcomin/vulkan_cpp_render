#include "include/engine/commands.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/renderpass.hpp"

namespace engine {
    CommandDispatcher::CommandDispatcher(const VulkanContext *vulkan_context, VkCommandBufferLevel lvl) : 
        context(vulkan_context), command_buffer_begin_info{get_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)} 
    {
        VkCommandPoolCreateInfo command_pool_create_info = get_create_info(
            context->queue_family.graphics_family.value()
        );
        if(vkCreateCommandPool(context->device.logical, &command_pool_create_info, nullptr, &command_pool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool");

        VkCommandBufferAllocateInfo command_buffer_alloc_info{};
        command_buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_alloc_info.pNext = nullptr;
        command_buffer_alloc_info.commandPool = command_pool;
        command_buffer_alloc_info.commandBufferCount = 1;
        command_buffer_alloc_info.level = lvl; //this is the buffer that we will submit as secondary to the main frame buffer

        if (vkAllocateCommandBuffers(context->device.logical, &command_buffer_alloc_info, &command_buffer) != VK_SUCCESS)
            throw std::runtime_error("cannot allocate main command buffer");
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

    inline VkCommandBufferBeginInfo CommandDispatcher::get_begin_info(VkCommandBufferUsageFlags flags) {
        return VkCommandBufferBeginInfo {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            nullptr,
            flags,
            nullptr
        };
    };

    inline VkCommandPoolCreateInfo CommandDispatcher::get_create_info(uint32_t queue_fam_index, VkCommandPoolCreateFlags flags) {
        return VkCommandPoolCreateInfo {
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            nullptr,
            flags,
            queue_fam_index
        };
    }

    inline VkCommandBufferAllocateInfo CommandDispatcher::get_alloc_info(VkCommandPool &pool, uint32_t count, VkCommandBufferLevel level) {
        return VkCommandBufferAllocateInfo {

        };
    }

    CommandDispatcher::~CommandDispatcher() {
        if(command_pool)
            vkDestroyCommandPool(context->device.logical, command_pool, nullptr);
    }
}