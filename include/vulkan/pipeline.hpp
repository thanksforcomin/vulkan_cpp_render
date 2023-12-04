//
//A big builder for graphical pipeline
//
#pragma once

#include <vulkan/vulkan.hpp>

#include <vector>
#include <functional>

namespace vulkan {
    namespace pipeline {
        struct pipeline_builder {
            VkGraphicsPipelineCreateInfo create_info;

            pipeline_builder& vertex_assembly(VkPipelineVertexInputStateCreateInfo&& info);
            pipeline_builder& input_assembly(VkPipelineInputAssemblyStateCreateInfo&& info);
            pipeline_builder& tesselation(VkPipelineTessellationStateCreateInfo&& info);
            pipeline_builder& viewport(VkPipelineViewportStateCreateInfo&& info);
            pipeline_builder& rasterization(VkPipelineRasterizationStateCreateInfo&& info);
            pipeline_builder& multisampling(VkPipelineMultisampleStateCreateInfo&& info);
            pipeline_builder& depth_stencil(VkPipelineDepthStencilStateCreateInfo&& info);
            pipeline_builder& color_blend(VkPipelineColorBlendStateCreateInfo&& info);
            pipeline_builder& dynamic_state(VkPipelineDynamicStateCreateInfo&& info);
            pipeline_builder& shader_stages(std::vector<VkPipelineShaderStageCreateInfo> stages);

            VkPipeline init(VkDevice &dev);

            private:
                VkPipelineVertexInputStateCreateInfo _vertex_assembly;
                VkPipelineInputAssemblyStateCreateInfo _input_assembly;
                VkPipelineTessellationStateCreateInfo _tesselation;
                VkPipelineViewportStateCreateInfo _viewport;
                VkPipelineRasterizationStateCreateInfo _rasterization;
                VkPipelineMultisampleStateCreateInfo _multisampling;
                VkPipelineDepthStencilStateCreateInfo _depth_stencil;
                VkPipelineColorBlendStateCreateInfo _color_blend;
                VkPipelineDynamicStateCreateInfo _dynamic_state;
                std::vector<VkPipelineShaderStageCreateInfo> _shader_stages;

        };

        VkPipelineVertexInputStateCreateInfo vertex_assembly_info(const VkVertexInputBindingDescription *bind_desc, const VkVertexInputAttributeDescription *attrib_desc, uint32_t attrib_size);
        VkPipelineInputAssemblyStateCreateInfo input_assembly_info(VkPrimitiveTopology topology);
        VkPipelineTessellationStateCreateInfo tesselation_info();
        VkPipelineViewportStateCreateInfo viewport_info(VkViewport *viewport, VkRect2D *scissors);
        VkPipelineRasterizationStateCreateInfo rasterization_info();
        VkPipelineMultisampleStateCreateInfo multisampling_info();
        VkPipelineDepthStencilStateCreateInfo depth_stencil_info();
        VkPipelineColorBlendAttachmentState color_blend_attachment();
        VkPipelineColorBlendStateCreateInfo color_blend_state(VkPipelineColorBlendAttachmentState *attachment);
        VkPipelineDynamicStateCreateInfo dynamic_state(VkDynamicState *states, uint32_t state_count);
        VkPipelineShaderStageCreateInfo shader_state(VkShaderStageFlagBits flag, const VkShaderModule &mod);
    }
}