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

            void begin(VkRenderPassBeginInfo &render_pass_begin_info);
            void end();
            void reset();

            const VkCommandBuffer command_buffer;

        private:
            VkCommandBufferBeginInfo command_buffer_begin_info;
            VkCommandBufferLevel command_buffer_level;
    };

    class CommandDispatcher {
        private:
            VulkanContext *context;

        public:
            CommandDispatcher(VulkanContext *vulkan_context, VkCommandBufferLevel lvl = VK_COMMAND_BUFFER_LEVEL_SECONDARY);
            CommandDispatcher(CommandDispatcher const& cmd) = default;
            CommandDispatcher(CommandDispatcher &&cmd);
            ~CommandDispatcher();

            void reset();
            void begin(const RenderPass &rp);
            void end();

            VkCommandPool command_pool;
            VkCommandBuffer command_buffer;
            VkCommandBufferBeginInfo command_buffer_begin_info;
    };
}