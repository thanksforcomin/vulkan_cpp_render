#pragma once

#include "include/engine/details.hpp"

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

            void begin(VkRenderPassBeginInfo &render_pass_begin_info);
            void end();
            void reset();

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