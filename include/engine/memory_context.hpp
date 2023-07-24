#pragma once

#include "include/engine/details.hpp"
#include "include/engine/commands.hpp"

namespace engine {
    class UploadContext {
        private:
            VulkanContext *context;

        public:
            UploadContext(VulkanContext *vulkan_context) noexcept;
            ~UploadContext();

            void immediate_submit();
        private:
            VkFence fence;
            CommandDispatcher command_dispatcher;
    };
}