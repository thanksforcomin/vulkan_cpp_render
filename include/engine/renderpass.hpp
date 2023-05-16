#pragma once
#include "include/engine/details.hpp"

namespace engine {
    class VulkanContext; //forward declaration

    class RenderPass {
        public:
            RenderPass(VulkanContext *vulkan_context, const uint32_t clear_val_count, VkClearValue *clear_val);
            ~RenderPass();

            void init(const uint32_t attachments_count,
                      VkAttachmentDescription *attachments,
                      const uint32_t subpass_count,
                      VkSubpassDescription *subpasses, 
                      const uint32_t dependencies_count = 0,
                      VkSubpassDependency *dependencies = nullptr);
            void init_default(); //one subpass, values taken from vulkan context
 
            VkRenderPass data;
            VkRenderPassBeginInfo begin_info;

        private:
            bool initialized;
            VulkanContext *context;
            const uint32_t clear_value_count;
            VkClearValue *clear_value;
    };
}