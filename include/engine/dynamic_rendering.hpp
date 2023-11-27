#pragma once
#include "include/engine/resources.hpp"

namespace engine {
    class DynamicRenderPass {
        private:
            friend class FrameGraph;

            VulkanContext *context;

            //prob a bad idea
            //we assign a new Id to each render pass, and store the next ID in a static variable
            static int next_id;

        public:
            DynamicRenderPass(VulkanContext *vulkan_context);
            ~DynamicRenderPass();

            void begin(VkCommandBuffer& cmd_buffer);
            void end(VkCommandBuffer& cmd_buffer);

            VkRenderingInfo rendering_info;

            void set_color_attachment(Attachment& attachment);
            void set_depth_attachment(Attachment& attachment, bool write_enabled = true);
            void set_stencil_attachment(Attachment& attachment);

        private:
            VkRenderingAttachmentInfoKHR color_attachment;
            VkRenderingAttachmentInfoKHR depth_attachment;
            VkRenderingAttachmentInfoKHR stencil_attachment;

        public:
            const int id;
    };
}