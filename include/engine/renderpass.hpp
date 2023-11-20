#pragma once

#include "include/engine/resources.hpp"

#include <vector>

namespace engine {
    class VulkanContext; //forward declaration

    class RenderPass {
        private:
            VulkanContext *context;
        public:
            RenderPass(VulkanContext *vulkan_context);
            ~RenderPass();

            void init_default(); //one subpass, values taken from vulkan context
            void init();
 
            VkRenderPass render_pass;
            VkRenderPassBeginInfo begin_info;

        private:
            bool initialized;
            std::vector<VkClearValue> clear_values;
    };

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