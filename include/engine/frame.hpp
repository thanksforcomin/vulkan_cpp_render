#pragma once

#include "include/engine/shader.hpp"
#include "include/engine/commands.hpp"
#include "include/engine/descriptor.hpp"
#include "include/engine/renderpass.hpp"

#include <functional>
#include <memory>

namespace engine {
    class VulkanContext; //forward declaration
    
    class Frame
    {
        private:
            VulkanContext *context;

        public:
            VkFence fence;
            VkSemaphore present_semaphore;
            VkSemaphore graphics_semaphore;

            //head command pool and head command buffer
            CommandPool command_pool;
            CommandBuffer command_buffer;

            Frame(VulkanContext *vulkan_context);
            Frame(const Frame &fr);
            Frame(Frame &&fr);
            ~Frame();

            void wait_for_fence();
            void reset_command_buffer();
    };

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