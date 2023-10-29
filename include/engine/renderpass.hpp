#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
}