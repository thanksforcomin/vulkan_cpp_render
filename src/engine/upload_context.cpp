#include "include/engine/upload_context.hpp"

#include "include/vulkan/vulkan_utils.hpp"

namespace engine {
    UploadContext::UploadContext(VulkanContext *vulkan_context) :
        context(vulkan_context),
        fence(vulkan::create_fence(context->device.logical)),
        cmd_pool(context, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT),
        cmd_buffer(context, cmd_pool.command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    { } 

    UploadContext::~UploadContext() {
        vkDestroyFence(context->device.logical, fence, nullptr);
    }

    void UploadContext::immediate_submit(std::function<void(VkCommandBuffer& cmd)>&& function) {
        cmd_buffer.begin();
        function(cmd_buffer.command_buffer);
        cmd_buffer.end();
        VkSubmitInfo submit_info(vulkan::get_submit_info(cmd_buffer.command_buffer));
        vulkan::submit_command_buffer(context->graphics_queue, &submit_info, fence);
        vkWaitForFences(context->device.logical, 1, &fence, true, 1000000000);
        vkResetFences(context->device.logical, 1, &fence);
        cmd_pool.reset();
    }
}