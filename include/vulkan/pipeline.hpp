#pragma once

#include <vulkan/vulkan.hpp>

#include <vector>
#include <functional>

namespace vulkan {
    struct pipeline_builder {
        VkGraphicsPipelineCreateInfo create_info;

        std::vector<std::function<void()>> destructors;

        pipeline_builder& vertex_assembly(VkPipelineVertexInputStateCreateInfo info);
        pipeline_builder& input_assembly(VkPipelineInputAssemblyStateCreateInfo info);
        pipeline_builder& tesselation(VkPipelineTessellationStateCreateInfo info);
        pipeline_builder& viewport(VkPipelineViewportStateCreateInfo info);
        pipeline_builder& rasterization(VkPipelineRasterizationStateCreateInfo info);
        pipeline_builder& multisampling(VkPipelineMultisampleStateCreateInfo info);
        pipeline_builder& depth_stencil(VkPipelineDepthStencilStateCreateInfo info);
        pipeline_builder& color_blend(VkPipelineColorBlendStateCreateInfo info);
        pipeline_builder& dynamic_state(VkPipelineDynamicStateCreateInfo info);
        pipeline_builder& shader_stages(std::vector<VkPipelineShaderStageCreateInfo> stages);

        VkPipeline init(VkDevice &dev);
    };
}