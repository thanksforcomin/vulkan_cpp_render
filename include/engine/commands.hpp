#pragma once

#include "include/engine/details.hpp"

namespace engine {
    class VulkanContext; //forward decl
    class RenderPass;

    class CommandDispatcher {
        public:
            CommandDispatcher(const VulkanContext *vulkan_context, VkCommandBufferLevel lvl = VK_COMMAND_BUFFER_LEVEL_SECONDARY);
            CommandDispatcher(CommandDispatcher const& cmd) = default;
            CommandDispatcher(CommandDispatcher &&cmd);
            ~CommandDispatcher();

            void reset();
            void begin(const RenderPass &rp);
            void end();

            VkCommandPool command_pool;
            VkCommandBuffer command_buffer;
            VkCommandBufferBeginInfo command_buffer_begin_info;

        private:
            const VulkanContext *context;
    };
}