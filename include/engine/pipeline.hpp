#pragma once

#ifndef __PIPELINE_H__
#define __PIPELINE_H__
#include "include/engine/shader.hpp"
#include "include/engine/commands.hpp"

namespace engine {
    class VulkanContext; //forward decl

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
            CommandDispatcher command_dispatcher;

            Frame(const VulkanContext *vulkan_context);
            Frame(const Frame &fr);
            Frame(Frame &&fr) = default;
            ~Frame();

            void wait_for_fence();
            void reset_command_buffer();

        private:
            const VulkanContext *context;
    };
}
#endif