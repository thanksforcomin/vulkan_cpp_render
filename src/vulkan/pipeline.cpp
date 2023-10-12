#include "include/vulkan/pipeline.hpp"

namespace vulkan {
    namespace pipeline {
        pipeline_builder& pipeline_builder::vertex_assembly(VkPipelineVertexInputStateCreateInfo info) {
            _vertex_assembly = info;
            create_info.pVertexInputState = &_vertex_assembly;
        }

        pipeline_builder& pipeline_builder::input_assembly(VkPipelineInputAssemblyStateCreateInfo info) {
            _input_assembly = info;
            create_info.pInputAssemblyState = &_input_assembly;
        }

        pipeline_builder& pipeline_builder::tesselation(VkPipelineTessellationStateCreateInfo info) {
            _tesselation = info;
            create_info.pTessellationState = &_tesselation;
        }

        pipeline_builder& pipeline_builder::viewport(VkPipelineViewportStateCreateInfo info) {
            _viewport = info;
            create_info.pViewportState = &_viewport;
        }    

        pipeline_builder& pipeline_builder::rasterization(VkPipelineRasterizationStateCreateInfo info) {
            _rasterization = info;
            create_info.pRasterizationState = &_rasterization;
        }

        pipeline_builder& pipeline_builder::multisampling(VkPipelineMultisampleStateCreateInfo info) {
            _multisampling = info;
            create_info.pMultisampleState = &_multisampling;
        }

        pipeline_builder& pipeline_builder::depth_stencil(VkPipelineDepthStencilStateCreateInfo info) {
            _depth_stencil = info;
            create_info.pDepthStencilState = &_depth_stencil;
        } 

        pipeline_builder& pipeline_builder::color_blend(VkPipelineColorBlendStateCreateInfo info) {
            _color_blend = info;
            create_info.pColorBlendState = &_color_blend;
        }

        pipeline_builder& pipeline_builder::dynamic_state(VkPipelineDynamicStateCreateInfo info) {
            create_info.pDynamicState = new VkPipelineDynamicStateCreateInfo(info);
        }

        pipeline_builder& pipeline_builder::shader_stages(std::vector<VkPipelineShaderStageCreateInfo> stages) {
            _shader_stages = stages;
            create_info.stageCount = _shader_stages.size();
            create_info.pStages = _shader_stages.data();
        }


        VkPipeline pipeline_builder::init(VkDevice &dev) {
            VkPipeline data;
            if(vkCreateGraphicsPipelines(dev, VK_NULL_HANDLE, 1, &create_info, nullptr, &data) != VK_SUCCESS) {
                throw std::runtime_error("failed to create graphics pipeline!");
            }
        };

        // pipeline stages create info

        VkPipelineVertexInputStateCreateInfo vertex_assembly_info(const VkVertexInputBindingDescription *bind_desc, const VkVertexInputAttributeDescription *attrib_desc, uint32_t attrib_size) {
            return VkPipelineVertexInputStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .pNext = nullptr,
                .vertexBindingDescriptionCount = 1,
                .pVertexBindingDescriptions = bind_desc,
                .vertexAttributeDescriptionCount = attrib_size,
                .pVertexAttributeDescriptions = attrib_desc
            };
        }

        VkPipelineInputAssemblyStateCreateInfo input_assembly_info(VkPrimitiveTopology topology) {
            return VkPipelineInputAssemblyStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .pNext = nullptr,
                .topology = topology,
                .primitiveRestartEnable = VK_FALSE
            };
        }

        VkPipelineViewportStateCreateInfo viewport_info(VkViewport *viewport, VkRect2D *scissors) {
            return VkPipelineViewportStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .pNext = nullptr,
                .viewportCount = 1,
                .pViewports = viewport,
                .scissorCount = 1,
                .pScissors = scissors
            };
        }

        VkPipelineRasterizationStateCreateInfo rasterization_info() {
            return VkPipelineRasterizationStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .pNext = nullptr,
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = VK_POLYGON_MODE_FILL,
                .cullMode = VK_CULL_MODE_BACK_BIT,
                .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
                .depthBiasEnable = VK_FALSE,
                .depthBiasConstantFactor = 0.0f,
                .depthBiasClamp = 0.0f,
                .depthBiasSlopeFactor = 0.0f,
                .lineWidth = 1.0f
            };
        }

        VkPipelineMultisampleStateCreateInfo multisampling_info() {
            return VkPipelineMultisampleStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .pNext = nullptr,
                .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                .sampleShadingEnable = VK_FALSE,
                .minSampleShading = 1.0f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = VK_FALSE,
                .alphaToOneEnable = VK_FALSE  
            };
        }

        VkPipelineDepthStencilStateCreateInfo depth_stencil_info() {
            return VkPipelineDepthStencilStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .pNext = nullptr,
                .depthTestEnable = VK_TRUE,
                .depthWriteEnable = VK_FALSE,
                .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable = VK_FALSE
            };
        }

        VkPipelineColorBlendAttachmentState color_blend_attachment() {
            return VkPipelineColorBlendAttachmentState {
                .blendEnable = VK_TRUE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };
        }

        VkPipelineColorBlendStateCreateInfo color_blend_state(VkPipelineColorBlendAttachmentState *attachment) {
            return VkPipelineColorBlendStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .pNext = nullptr,
                .attachmentCount = 1,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .pAttachments = attachment,
                .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
            };
        }

        VkPipelineDynamicStateCreateInfo dynamic_state(VkDynamicState *states, uint32_t state_count) {
            return VkPipelineDynamicStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .pNext = nullptr,
                .dynamicStateCount = state_count,
                .pDynamicStates = states
            };
        }

        VkPipelineShaderStageCreateInfo shader_state(VkShaderStageFlagBits flag, const VkShaderModule &mod) {
            VkPipelineShaderStageCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            create_info.stage = flag;
            create_info.module = mod;
            create_info.pName = "main"; //because why tf would you want to have a starting point not at main?
            return create_info;
        }
    }
}