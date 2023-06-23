#pragma once

#ifndef __PIPELINE_H__
#define __PIPELINE_H__
#include "include/engine/shader.hpp"
#include "include/engine/commands.hpp"
#include "include/engine/descriptor.hpp"
#include "include/engine/renderpass.hpp"

namespace engine {
    class VulkanContext; //forward decl

    class Pipeline {

        private:
            struct StagesInfo {
                VkPipelineVertexInputStateCreateInfo vertex_input_state;
                VkPipelineInputAssemblyStateCreateInfo assembly_input_state;
                VkPipelineViewportStateCreateInfo viewport_state;
                VkPipelineRasterizationStateCreateInfo rasterization_state;
                VkPipelineMultisampleStateCreateInfo multisample_state;
                VkPipelineDepthStencilStateCreateInfo depth_stencil_state;
                VkPipelineColorBlendStateCreateInfo color_blend_state;
                VkPipelineDynamicStateCreateInfo dynamic_state;
                std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
            };

        public:
            Pipeline(VulkanContext *vulkan_context);
            ~Pipeline();
            void init(StagesInfo stages_info, RenderPass &render_pass);

        private:

            StagesInfo get_default_create_info(Shader &vert_shader, Shader &frag_shader, std::vector<VkDescriptorSetLayout>& sets);
            StagesInfo get_depth_create_info(Shader &vert_shader, Shader &frag_shader, std::vector<VkDescriptorSetLayout>& sets);

            const VulkanContext *context;

            void create_layout(std::vector<VkDescriptorSetLayout>& sets); //TODO: i really want to redesign the VulkanContext class because i don't do initialisation like in there anymore
        
        public:
            VkPipelineLayout pipeline_layout;
            VkPipeline pipeline;
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
            Frame(Frame &&fr);
            ~Frame();

            void wait_for_fence();
            void reset_command_buffer();

        private:
            const VulkanContext *context;
    };
}
#endif