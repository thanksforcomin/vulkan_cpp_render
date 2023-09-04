#pragma once
#include "include/engine/details.hpp"

namespace engine {
    class VulkanContext; //forward declaration

    class RenderPass {
        private:
            VulkanContext *context;
        public:
            RenderPass(VulkanContext *vulkan_context);
            ~RenderPass();

            void init_default(); //one subpass, values taken from vulkan context
 
            VkRenderPass render_pass;
            VkRenderPassBeginInfo begin_info;

        private:
            bool initialized;
            std::vector<VkClearValue> clear_values;
    };
}