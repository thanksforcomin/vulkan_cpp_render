#include "include/engine/pipeline.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {
    PipelineConfiguration::PipelineConfiguration(VulkanContext *vulkan_context, Shader &vert_shader, Shader &frag_shader) : 
        context(vulkan_context),
        shader_stages{vert_shader.stage_create_info, frag_shader.stage_create_info},
        viewports {0.0f, 0.0f, 
            (float)context->swap_chain.swap_chain_extent.width, 
            (float)context->swap_chain.swap_chain_extent.height,
            0.0f,
            1.0f },
        scissors {
            {0, 0},
            context->swap_chain.swap_chain_extent
        },
        vertex_input_state(
            new VkPipelineVertexInputStateCreateInfo{defaults::_vertex_input_state_create_info}
        ),
        input_assembly_state(
            new VkPipelineInputAssemblyStateCreateInfo{defaults::_assembly_input_state_create_info}
        ),
        viewport_state(
            new VkPipelineViewportStateCreateInfo{defaults::_get_viewport_state_create_info(&viewports, &scissors)}
        ),
        rasterization_state(
            new VkPipelineRasterizationStateCreateInfo{defaults::_rasterization_state_create_info}
        ),
        multisample_state(
            new VkPipelineMultisampleStateCreateInfo{defaults::_multisample_state_create_info}
        ),
        depth_stencil_state(
            new VkPipelineDepthStencilStateCreateInfo{defaults::_depth_and_stencil_state_create_info} 
        ),
        color_blend_state (
            new VkPipelineColorBlendStateCreateInfo{defaults::_color_blend_attachment_state_create_info}
        )
    { }
}

namespace engine {


    Pipeline::Pipeline(VulkanContext *vulkan_context) : context(vulkan_context) { }

    void Pipeline::init(PipelineConfiguration &config, RenderPass &render_pass) {
        VkGraphicsPipelineCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        create_info.stageCount = 2;
        create_info.pStages = &config.shader_stages[0];
        create_info.pVertexInputState = config.vertex_input_state.get();
        create_info.pInputAssemblyState = config.input_assembly_state.get();
        create_info.pViewportState = config.viewport_state.get();
        create_info.pRasterizationState = config.rasterization_state.get();
        create_info.pMultisampleState = config.multisample_state.get();
        create_info.pDepthStencilState = config.depth_stencil_state.get();
        create_info.pColorBlendState = config.color_blend_state.get();
        create_info.pDynamicState = config.dynamic_state_info.get();
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