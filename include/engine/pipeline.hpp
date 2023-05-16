#pragma once

#ifndef __PIPELINE_H__
#define __PIPELINE_H__
#include "include/engine/details.hpp"
#include "include/engine/shader.hpp"

namespace engine {
    class Pipeline {
        public:
            Pipeline(Shader &vert_shader, Shader &frag_shader, VulkanContext *vulkan_context);
            ~Pipeline();

        private:
            void create_pipeline_layout(Shader &vert_shader, Shader &frag_shader);

            const VulkanContext *context;
            VkPipelineLayout pipeline_layout;
    };

    class Frame
    {
        public:
            VkFence fence;
            VkSemaphore present_semaphore;
            VkSemaphore graphics_semaphore;

            //head command pool and head command buffer
            VkCommandBuffer command_buffer;
            VkCommandPool command_pool;

            Frame(const VulkanContext *vulkan_context);
            ~Frame();   

        private:
            const VulkanContext *context;
    };
}
#endif