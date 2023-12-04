#pragma once

#include "include/vulkan/pipeline.hpp"
#include "include/vulkan/vertex.hpp"
#include "include/vulkan/vulkan_utils.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader.hpp"

namespace fwd_plus {
    VkPipeline create_classic_pipeline(engine::VulkanContext &context,
                                       VkPipelineLayout &pipeline_layout, 
                                       engine::Shader &vertex_shader, 
                                       engine::Shader &fragment_shader);
    VkPipeline create_depth_pipeline(engine::VulkanContext &context, 
                                     VkPipelineLayout pipeline_layout,
                                     engine::Shader &vertex_shader);
}