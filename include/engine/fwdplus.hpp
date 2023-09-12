#pragma once

#include "include/vulkan/pipeline.hpp"
#include "include/vulkan/vertex.hpp"
#include "include/vulkan/vulkan_utils.hpp"
#include "include/engine/vulkan_context.hpp"

namespace fwd_plus {
    VkPipeline create_classic_pipeline(engine::VulkanContext &context,
                                       VkRenderPass &render_pass, 
                                       VkPipelineLayout &pipeline_layout, 
                                       VkPipelineShaderStageCreateInfo &vertex_shader, 
                                       VkPipelineShaderStageCreateInfo &fragment_shader);
    VkPipeline create_depth_pipeline(engine::VulkanContext &context, 
                                     VkRenderPass &render_pass, 
                                     VkPipelineLayout pipeline_layout,
                                     VkPipelineShaderStageCreateInfo &vertex_shader);
}