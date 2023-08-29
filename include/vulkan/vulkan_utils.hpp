#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"

namespace vulkan {
    std::vector<const char*> require_extensions();

    bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface);

    bool device_extensions_support(VkPhysicalDevice &dev, std::vector<const char*> extensions);

    bool validation_layers_support(std::vector<const char *> validation_layers);

    swap_chain_support_details get_swap_chain_support(VkPhysicalDevice &dev, VkSurfaceKHR &surface);
    
    queue_family_indicies find_queue_family(VkPhysicalDevice &dev, VkSurfaceKHR &surface);

    VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes);

    VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);
            
    VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats); 

    std::vector<VkImage> get_swap_chain_images(VkDevice &dev, VkSwapchainKHR &swap_chain);

    VkCommandBufferBeginInfo get_command_buffer_begin_info(VkCommandBufferUsageFlags flags = 0);

    VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlagBits shader_stage, uint32_t binding_point);

    VkDescriptorBufferInfo get_buffer_info(VkBuffer &buffer, VkDeviceSize size, uint32_t offset = 0);

    VkWriteDescriptorSet get_descriptor_write_info(VkDescriptorType type, VkDescriptorSet dst_set, uint32_t binding, VkDescriptorBufferInfo &buffer_info);

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

    pipeline_builder begin_pipeline_builder(VkRenderPass &render_pass, uint32_t subpass = 0);
}
