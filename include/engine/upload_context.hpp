#pragma once

#include "include/engine/vulkan_context.hpp"
#include "include/engine/commands.hpp"

#include <functional>

namespace engine {
    class UploadContext {
        VulkanContext *context;
    
    public:
        UploadContext() = delete;
        UploadContext(VulkanContext *vulkan_context);
        ~UploadContext();

        void immediate_submit(std::function<void(VkCommandBuffer& cmd)>&& function);

    private:
        VkFence fence;
        CommandPool cmd_pool;
        CommandBuffer cmd_buffer;
    };
}