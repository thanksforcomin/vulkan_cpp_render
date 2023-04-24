#pragma once
#include "include/engine/details.hpp"
#include "include/engine/shader.hpp"

namespace engine {
    class VulkanContext; //forward declaration

    class Pipeline {
        public:
            Pipeline(Shader &vert_shader, Shader &frag_shader, VulkanContext *vulkan_context);
            ~Pipeline();

        private:
            void create_pipeline_layout(Shader &vert_shader, Shader &frag_shader);

            VulkanContext *context;
            VkPipelineLayout pipeline_layout;
    };

    class RenderPass {
        public:
            RenderPass() = delete;
            RenderPass();

            void init();

        private:
            VulkanContext *context;
    };

    struct Frame
    {
            VkFence fence;
            VkSemaphore semaphore;
    };
}