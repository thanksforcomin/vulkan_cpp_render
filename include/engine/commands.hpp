#pragma once

#include "include/engine/details.hpp"

namespace engine {
    class VulkanContext; //forward decl

    class CommandDiscpather {
        public:
            CommandDiscpather(const VulkanContext *vulkan_context);
            ~CommandDiscpather();

            void switch_frames();
            void reset();

        private:
            const VulkanContext *context;

            VkCommandPool command_pool;
            VkCommandBuffer command_buffer;
            VkCommandBufferBeginInfo command_buffer_begin_info;
    };
}