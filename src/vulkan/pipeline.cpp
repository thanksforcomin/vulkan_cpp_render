#include "include/vulkan/pipeline.hpp"

namespace vulkan {
    pipeline_builder& pipeline_builder::vertex_assembly(VkPipelineVertexInputStateCreateInfo info) {
        create_info.pVertexInputState = new VkPipelineVertexInputStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pVertexInputState; });
    }

    pipeline_builder& pipeline_builder::input_assembly(VkPipelineInputAssemblyStateCreateInfo info) {
        create_info.pInputAssemblyState = new VkPipelineInputAssemblyStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pInputAssemblyState; });
    }

    pipeline_builder& pipeline_builder::tesselation(VkPipelineTessellationStateCreateInfo info) {
        create_info.pTessellationState = new VkPipelineTessellationStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pTessellationState; });
    }

    pipeline_builder& pipeline_builder::viewport(VkPipelineViewportStateCreateInfo info) {
        create_info.pViewportState = new VkPipelineViewportStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pViewportState; });
    }    

    pipeline_builder& pipeline_builder::rasterization(VkPipelineRasterizationStateCreateInfo info) {
        create_info.pRasterizationState = new VkPipelineRasterizationStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pRasterizationState; });
    }

    pipeline_builder& pipeline_builder::multisampling(VkPipelineMultisampleStateCreateInfo info) {
        create_info.pMultisampleState = new VkPipelineMultisampleStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pMultisampleState; });
    }

    pipeline_builder& pipeline_builder::depth_stencil(VkPipelineDepthStencilStateCreateInfo info) {
        create_info.pDepthStencilState = new VkPipelineDepthStencilStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pDepthStencilState; });
    } 

    pipeline_builder& pipeline_builder::color_blend(VkPipelineColorBlendStateCreateInfo info) {
        create_info.pColorBlendState = new VkPipelineColorBlendStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pColorBlendState; });
    }

    pipeline_builder& pipeline_builder::dynamic_state(VkPipelineDynamicStateCreateInfo info) {
        create_info.pDynamicState = new VkPipelineDynamicStateCreateInfo(info);
        destructors.push_back([=]{ delete create_info.pDynamicState; });
    }

    pipeline_builder& pipeline_builder::shader_stages(std::vector<VkPipelineShaderStageCreateInfo> stages) {
        create_info.stageCount = stages.size();
        create_info.pStages = stages.data();
    }

    VkPipeline pipeline_builder::init(VkDevice &dev) {
        VkPipeline data;
        if(vkCreateGraphicsPipelines(dev, VK_NULL_HANDLE, 1, &create_info, nullptr, &data) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        for(auto& i : destructors) {
                i();
        }
    };
}