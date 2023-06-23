#include "include/engine/pipeline.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {
    Pipeline::Pipeline(VulkanContext *vulkan_context) : context(vulkan_context)
    {
        //create_pipeline_layout(vert_shader, frag_shader);
    }

    void Pipeline::init(StagesInfo stages_info, RenderPass &render_pass) {

        VkGraphicsPipelineCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        create_info.stageCount = 2;
        create_info.pStages = &stages_info.shader_stages[0];
        create_info.pVertexInputState = &stages_info.vertex_input_state;
        create_info.pInputAssemblyState = &stages_info.assembly_input_state;
        create_info.pViewportState = &stages_info.viewport_state;
        create_info.pRasterizationState = &stages_info.rasterization_state;
        create_info.pMultisampleState = &stages_info.multisample_state;
        create_info.pDepthStencilState = &stages_info.depth_stencil_state;
        create_info.pColorBlendState = &stages_info.color_blend_state;
        create_info.pDynamicState = &stages_info.dynamic_state;
        create_info.layout = pipeline_layout;
        create_info.renderPass = render_pass.data;
        create_info.subpass = 9;
        create_info.basePipelineHandle = VK_NULL_HANDLE;
        create_info.basePipelineIndex = -1; //i have no idea what this line is doing
        create_info.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;

        if(vkCreateGraphicsPipelines(context->device.logical, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS) 
            throw std::runtime_error("failed to create graphics pipeline");
        std::cout << "pipeline allocated\n";

    }

    Pipeline::StagesInfo Pipeline::get_default_create_info(Shader &vert_shader, Shader &frag_shader, std::vector<VkDescriptorSetLayout>& sets) {
        VkPipelineShaderStageCreateInfo shader_stages_create_info[] = {vert_shader.stage_create_info, frag_shader.stage_create_info};

        //dynamic states
        std::vector<VkDynamicState> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_info{};
        dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_info.dynamicStateCount = (uint32_t)dynamic_states.size();
        dynamic_state_info.pDynamicStates = &dynamic_states[0];

        //assembly input
        VkPipelineInputAssemblyStateCreateInfo input_info;
        input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_info.primitiveRestartEnable = VK_FALSE;

        //vertex input
        VkPipelineVertexInputStateCreateInfo vertex_input_info{};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_info.vertexBindingDescriptionCount = 1;
        vertex_input_info.pVertexBindingDescriptions = &vertex::bind_description;
        vertex_input_info.vertexAttributeDescriptionCount = (uint32_t)4;
        vertex_input_info.pVertexAttributeDescriptions = &vertex::attrib_descriptions[0];

        //viewport settings
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)context->swap_chain.swap_chain_extent.width;
        viewport.height = (float)context->swap_chain.swap_chain_extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissors{};
        scissors.extent = context->swap_chain.swap_chain_extent;
        scissors.offset = {0, 0};

        VkPipelineViewportStateCreateInfo viewport_state_info{};
        viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_info.scissorCount = 1;
        viewport_state_info.pScissors = &scissors;
        viewport_state_info.viewportCount = 1;
        viewport_state_info.pViewports = &viewport;

        //rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizer_info{};
        rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer_info.depthClampEnable = VK_FALSE;
        rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
        rasterizer_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer_info.lineWidth = 1.0f;
        rasterizer_info.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer_info.depthBiasEnable = VK_FALSE;
        rasterizer_info.depthBiasConstantFactor = 0.0f; //we won't use it
        rasterizer_info.depthBiasClamp = 0.0f; //we won't use it
        rasterizer_info.depthBiasSlopeFactor = 0.0f; //we won't use it

        //(no) multisampling
        //стас если ты это читаешь это пиздец
        VkPipelineMultisampleStateCreateInfo multisampling_info{};
        multisampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling_info.sampleShadingEnable = VK_FALSE;
        multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling_info.minSampleShading = 1.0f; //we won't use it
        multisampling_info.pSampleMask = nullptr; //we won't use it
        multisampling_info.alphaToCoverageEnable = VK_FALSE; //we won't use it
        multisampling_info.alphaToOneEnable = VK_FALSE; //we won't use it

        //depth and stencil
        VkPipelineDepthStencilStateCreateInfo depth_stencil_info{};
        depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil_info.depthTestEnable = VK_TRUE;
        depth_stencil_info.depthWriteEnable = VK_FALSE; //we will have a depth prepass
        depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; //we will have a depth prepass
        depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
        depth_stencil_info.stencilTestEnable = VK_FALSE; //who needs this anyway

        //color blending
        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment.blendEnable = VK_TRUE;
        color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_info{};
        color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_info.logicOpEnable = VK_FALSE;
        color_blend_info.logicOp = VK_LOGIC_OP_COPY; //we won't need it
        color_blend_info.attachmentCount = 1;
        color_blend_info.pAttachments = &color_blend_attachment;
        color_blend_info.blendConstants[0] = 0.0f; //we won't need it
        color_blend_info.blendConstants[1] = 0.0f; //we won't need it
        color_blend_info.blendConstants[2] = 0.0f; //we won't need it
        color_blend_info.blendConstants[3] = 0.0f; //we won't need it

        StagesInfo stages_info{};
        stages_info.assembly_input_state = input_info;
        stages_info.vertex_input_state = vertex_input_info;
        stages_info.viewport_state = viewport_state_info;
        stages_info.rasterization_state = rasterizer_info;
        stages_info.multisample_state = multisampling_info;
        stages_info.depth_stencil_state = depth_stencil_info;
        stages_info.color_blend_state = color_blend_info;
        stages_info.dynamic_state = dynamic_state_info;
        stages_info.shader_stages = std::vector<VkPipelineShaderStageCreateInfo> {shader_stages_create_info[0], shader_stages_create_info[1]}; //TODO: fix
    }

    void Pipeline::create_layout(std::vector<VkDescriptorSetLayout>& sets) {
        VkPipelineLayoutCreateInfo pipeline_layout_info;
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = sets.size();
        pipeline_layout_info.pSetLayouts = &sets[0];
        pipeline_layout_info.pPushConstantRanges = 0; //TODO: add push constants
        pipeline_layout_info.pushConstantRangeCount = 0;

        if(vkCreatePipelineLayout(context->device.logical, &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS) 
            throw std::runtime_error("failed to create pipeline layout");
        std::cout << "created pipeline layout (don't forget to init it)\n";
    }        
}

namespace engine {
    Frame::Frame(const VulkanContext *vulkan_context) : context(vulkan_context), command_dispatcher(vulkan_context, VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
        VkFenceCreateInfo fence_create_info{};
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_create_info.pNext = nullptr;
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if(vkCreateFence(context->device.logical, &fence_create_info, nullptr, &fence) != VK_SUCCESS)
            throw std::runtime_error("failed to create fence");

        VkSemaphoreCreateInfo semop_create_info{};
        semop_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semop_create_info.pNext = nullptr;
        semop_create_info.flags = 0;
        if (vkCreateSemaphore(context->device.logical, &semop_create_info, nullptr, &present_semaphore) != VK_SUCCESS)
            throw std::runtime_error("failed to create semaphore");
        if(vkCreateSemaphore(context->device.logical, &semop_create_info, nullptr, &graphics_semaphore) != VK_SUCCESS)
            throw std::runtime_error("failed to create semaphore");

        std::cout << "frame created\n";
    }

    Frame::Frame(const Frame &fr) : 
        fence{fr.fence},
        present_semaphore{fr.present_semaphore},
        graphics_semaphore{fr.graphics_semaphore},
        command_dispatcher(fr.command_dispatcher),
        context(fr.context)
    {
        
    }

    Frame::Frame(Frame &&fr) : 
        fence{fr.fence},
        present_semaphore{fr.present_semaphore},
        graphics_semaphore{fr.graphics_semaphore},
        command_dispatcher(fr.command_dispatcher),
        context(std::move(fr.context))
    {
        fr.fence = NULL;
        fr.present_semaphore = NULL;
        fr.graphics_semaphore = NULL;
    }

    Frame::~Frame() {
        vkDestroyFence(context->device.logical, fence, nullptr);
        vkDestroySemaphore(context->device.logical, graphics_semaphore, nullptr);
        vkDestroySemaphore(context->device.logical, present_semaphore, nullptr);    
    }

    void Frame::wait_for_fence() {
        if(vkWaitForFences(context->device.logical, 1, &fence, true, 1000000000) != VK_SUCCESS)
            throw std::runtime_error("waiting for fence took too long");
        if(vkResetFences(context->device.logical, 1, &fence) != VK_SUCCESS)
            throw std::runtime_error("couldn't reset fence");
    }
}