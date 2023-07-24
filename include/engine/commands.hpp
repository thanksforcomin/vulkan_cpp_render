#pragma once

#include "include/engine/details.hpp"

namespace engine {
    class VulkanContext; //forward decl
    class RenderPass;

    class CommandBuffer {
        private:
            VulkanContext *context;

        public:
            CommandBuffer(VulkanContext *vulkan_context);
    };

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

            inline VkCommandBufferBeginInfo get_begin_info(VkCommandBufferUsageFlags flags = 0);
            inline VkCommandPoolCreateInfo get_create_info(uint32_t queue_fam_index, VkCommandPoolCreateFlags flags = 0);
            inline VkCommandBufferAllocateInfo get_alloc_info(VkCommandPool &pool, 
                                                              uint32_t count = 1, 
                                                              VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    };
}