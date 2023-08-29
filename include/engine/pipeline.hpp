#pragma once

#include "include/engine/shader.hpp"
#include "include/engine/commands.hpp"
#include "include/engine/descriptor.hpp"
#include "include/engine/renderpass.hpp"

#include <memory>

namespace engine {
    class VulkanContext; //forward decl

    class PipelineConfiguration {
        private:
            VulkanContext *context;

            std::vector<VkDynamicState> dynamic_states_array;
            VkViewport viewports;
            VkRect2D scissors;
        public:
            std::unique_ptr<VkPipelineVertexInputStateCreateInfo> vertex_input_state;
            std::unique_ptr<VkPipelineInputAssemblyStateCreateInfo> input_assembly_state;
            std::unique_ptr<VkPipelineViewportStateCreateInfo> viewport_state;
            std::unique_ptr<VkPipelineRasterizationStateCreateInfo> rasterization_state;
            std::unique_ptr<VkPipelineMultisampleStateCreateInfo> multisample_state;
            std::unique_ptr<VkPipelineDepthStencilStateCreateInfo> depth_stencil_state;
            std::unique_ptr<VkPipelineColorBlendStateCreateInfo> color_blend_state;
            std::unique_ptr<VkPipelineDynamicStateCreateInfo> dynamic_state_info;
            std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

        public:
            PipelineConfiguration(VulkanContext *vulkan_context, Shader &vert_shader, Shader &frag_shader);
            ~PipelineConfiguration();
    };

    class Pipeline { 
        public:
            Pipeline(VulkanContext *vulkan_context);
            ~Pipeline();
            void init(PipelineConfiguration &config, RenderPass &render_pass);

        private:

            //StagesInfo get_default_create_info(Shader &vert_shader, Shader &frag_shader, std::vector<VkDescriptorSetLayout>& sets);
            //StagesInfo get_depth_create_info(Shader &vert_shader, Shader &frag_shader, std::vector<VkDescriptorSetLayout>& sets);

            const VulkanContext *context;

            void create_layout(std::vector<VkDescriptorSetLayout>& sets); //TODO: i really want to redesign the VulkanContext class because i don't do initialisation like in there anymore
        
        public:
            VkPipelineLayout pipeline_layout;
            VkPipeline pipeline;
    };

    class Frame
    {
        private:
            VulkanContext *context;

        public:
            VkFence fence;
            VkSemaphore present_semaphore;
            VkSemaphore graphics_semaphore;

            //head command pool and head command buffer
            CommandPool command_pool;
            CommandBuffer command_buffer;

            Frame(VulkanContext *vulkan_context);
            Frame(const Frame &fr);
            Frame(Frame &&fr);
            ~Frame();

            void wait_for_fence();
            void reset_command_buffer();
    };
}