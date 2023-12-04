#include "include/engine/fwdplus.hpp"

namespace fwd_plus {
    VkPipeline create_classic_pipeline(engine::VulkanContext &context,
                                       VkPipelineLayout &pipeline_layout, 
                                       engine::Shader &vertex_shader, 
                                       engine::Shader &fragment_shader) 
    {
        VkPipelineColorBlendAttachmentState color_blend_attachment(vulkan::pipeline::color_blend_attachment());
        VkViewport viewport(vulkan::get_viewport(context.swap_chain.swap_chain_extent));
        VkRect2D scissors{
            .extent = context.swap_chain.swap_chain_extent,
            .offset = { 0, 0 }
        };
        VkDynamicState dynamic_states[2] { 
            VK_DYNAMIC_STATE_VIEWPORT, 
            VK_DYNAMIC_STATE_SCISSOR 
        };
        std::vector<VkPipelineShaderStageCreateInfo> shaders {
            vertex_shader.stage_create_info,
            fragment_shader.stage_create_info
        };

        //TODO: multiple subpass something something
        return vulkan::begin_pipeline_builder(pipeline_layout, 0)
        .vertex_assembly(vulkan::pipeline::vertex_assembly_info(&vulkan::vertex::bind_description, &vulkan::vertex::attrib_descriptions[0], 4))
        .input_assembly(vulkan::pipeline::input_assembly_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
        .viewport(vulkan::pipeline::viewport_info(&viewport, &scissors))
        .rasterization(vulkan::pipeline::rasterization_info())
        .multisampling(vulkan::pipeline::multisampling_info())
        .depth_stencil(vulkan::pipeline::depth_stencil_info())
        .color_blend(vulkan::pipeline::color_blend_state(&color_blend_attachment))
        .dynamic_state(vulkan::pipeline::dynamic_state(&dynamic_states[0], 0))
        .shader_stages(shaders)
        .init(context.device.logical);
    }

    VkPipeline create_depth_pipeline(engine::VulkanContext &context, 
                                     VkPipelineLayout pipeline_layout,
                                     engine::Shader &vertex_shader)
    {
        VkViewport viewport(vulkan::get_viewport(context.swap_chain.swap_chain_extent));
        VkRect2D scissors{
            .extent = context.swap_chain.swap_chain_extent,
            .offset = { 0, 0 }
        };
        return vulkan::begin_pipeline_builder(pipeline_layout, 0)
        .vertex_assembly(vulkan::pipeline::vertex_assembly_info(&vulkan::vertex::bind_description, &vulkan::vertex::attrib_descriptions[0], 4))
        .input_assembly(vulkan::pipeline::input_assembly_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
        .viewport(vulkan::pipeline::viewport_info(&viewport, &scissors))
        .rasterization(vulkan::pipeline::rasterization_info())
        .multisampling(vulkan::pipeline::multisampling_info())
        .depth_stencil(
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .pNext = nullptr,
                .depthTestEnable = VK_TRUE,
                .depthWriteEnable = VK_TRUE,
                .depthCompareOp = VK_COMPARE_OP_LESS,
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable = VK_FALSE
            }
        )
        .shader_stages({vertex_shader.stage_create_info})
        .init(context.device.logical);
    }
}