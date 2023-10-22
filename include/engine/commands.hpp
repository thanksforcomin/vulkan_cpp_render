#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace engine {
    class VulkanContext; //forward decl
    class RenderPass;

    class CommandBuffer {
        private:
            VulkanContext *context;
            VkCommandPool *pool;

        public:
            CommandBuffer(VulkanContext *vulkan_context, VkCommandPool command_pool, VkCommandBufferLevel lvl = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            CommandBuffer(CommandBuffer const &buff) = default;
            CommandBuffer(CommandBuffer &&buff);
            ~CommandBuffer();

            void begin();
            void begin_renderpass(VkRenderPassBeginInfo &render_pass_begin_info);
            void end_renderpass();
            void end();
            void reset();
            void submit(std::vector<VkSemaphore> wait_semop,
                        std::vector<VkSemaphore> signal_semop,
                        VkFence fence,
                        uint32_t wait_stages = 0);
            
            VkCommandBuffer command_buffer;

        private:
            VkCommandBufferBeginInfo command_buffer_begin_info;
            VkCommandBufferLevel command_buffer_level;
    };

    class CommandPool {
        private:
            VulkanContext *context;

        public: 
            CommandPool(VulkanContext *vulkan_context, VkCommandPoolCreateFlags flags = 0);
            CommandPool(CommandPool const &pool) = default;
            CommandPool(CommandPool &&pool);
            ~CommandPool();

            VkCommandPool command_pool;
    };
}