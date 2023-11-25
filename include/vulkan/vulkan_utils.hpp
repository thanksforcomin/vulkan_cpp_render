#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"
#include "include/vulkan/pipeline.hpp"
#include "include/vulkan/vertex.hpp"

namespace vulkan {
    std::vector<const char*> require_extensions();

    bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface, std::vector<const char*>& device_extensions);

    bool device_extensions_support(VkPhysicalDevice &dev, std::vector<const char*>& extensions);

    bool validation_layers_support(std::vector<const char *> validation_layers);

    swap_chain_support_details get_swap_chain_support(VkPhysicalDevice &dev, VkSurfaceKHR &surface);
    
    queue_family_indicies find_queue_family(VkPhysicalDevice &dev, VkSurfaceKHR &surface);

    VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes);

    VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);
            
    VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats); 

    VkFormat find_format(VkPhysicalDevice &dev, std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat find_depth_format(VkPhysicalDevice &dev);

    VkExtent3D extent_2d_to_3d(VkExtent2D extent);

    std::vector<VkImage> get_swap_chain_images(VkDevice &dev, VkSwapchainKHR &swap_chain);

    VkCommandBufferBeginInfo get_command_buffer_begin_info(VkCommandBufferUsageFlags flags = 0);

    VkRenderPassBeginInfo get_render_pass_begin_info(VkRenderPass &render_pass, VkRect2D render_area, VkClearValue *clear_value, uint32_t clear_value_size = 1);

    VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding_point);

    VkDescriptorBufferInfo get_buffer_info(VkBuffer &buffer, VkDeviceSize size, uint32_t offset = 0);

    VkWriteDescriptorSet get_descriptor_write_info(VkDescriptorType type, VkDescriptorSet dst_set, uint32_t binding, VkDescriptorBufferInfo &buffer_info);

    pipeline::pipeline_builder begin_pipeline_builder(VkPipelineLayout &layout, uint32_t subpass = 0);

    VkAttachmentDescription get_color_attachment(VkFormat &format, VkImageLayout fin_layout, VkImageLayout init_layout = VK_IMAGE_LAYOUT_UNDEFINED);

    VkAttachmentDescription get_depth_attachment(VkFormat &format, VkImageLayout fin_layout, VkImageLayout init_layout = VK_IMAGE_LAYOUT_UNDEFINED);

    VkSubpassDependency get_subpass_dependency(uint32_t dst_subpass);

    VkViewport get_viewport(VkExtent2D &extent);

    VkSubmitInfo get_submit_info(VkCommandBuffer &command_buffer,
                                 std::vector<VkSemaphore>& wait_semaphores,
                                 std::vector<VkSemaphore>& sig_semaphores,
                                 uint32_t& wait_stages);

    VkSubmitInfo get_submit_info(VkCommandBuffer &command_buffer);

    void submit_command_buffer(const VkQueue& queue, VkSubmitInfo* submit_info, VkFence& fence);
    
    void submit_frame(std::vector<VkCommandBuffer>&& command_buffers,
                      std::vector<VkSemaphore>&& wait_semaphores,
                      std::vector<VkSemaphore>&& sig_semaphores,
                      const VkFence& fence,
                      const VkQueue& queue,
                      VkPipelineStageFlags stage_flag = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                      );

    void present_frame(const VkSwapchainKHR &swap_chain, std::vector<VkSemaphore>&& wait_semops, uint32_t *index, const VkQueue& queue);

    VkPresentInfoKHR get_present_info(VkSwapchainKHR &swap_chain, uint32_t index);

    allocated_buffer allocate_buffer(VmaAllocator &allocator, VkDeviceSize size, VkBufferUsageFlags flags, VmaMemoryUsage usage);

    allocated_image allocate_image(VmaAllocator &allocator, VkExtent3D extent, VkFormat format, VkImageUsageFlags flags, VmaMemoryUsage usage);

    void upload_to_buffer(allocated_buffer &buffer, vertex::Vertex* data, uint32_t size);

    VkRenderingInfoKHR get_rendering_info(VkRect2D rendering_area, 
                                          VkRenderingAttachmentInfoKHR *color_attachment, 
                                          VkRenderingAttachmentInfoKHR *depth_attachment, 
                                          VkRenderingAttachmentInfoKHR *stencil_attachment,
                                          uint32_t layer_count = 1,
                                          uint32_t color_attachment_count = 1);
                                          
    VkRenderingAttachmentInfoKHR get_rendering_attachment_info(VkImageView &image_view, 
                                                               VkImageLayout image_layout,
                                                               VkClearValue clear_val = {0.0f, 0.0f, 0.0f, 1.0f},
                                                               VkAttachmentLoadOp load_op = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                               VkAttachmentStoreOp store_op = VK_ATTACHMENT_STORE_OP_STORE);

    VkPhysicalDeviceDynamicRenderingFeaturesKHR get_dynamic_rendering_features();

    void end_rendering();

    void change_image_layout(VkCommandBuffer &cmd_buffer, VkImage &image, VkImageLayout old_layout, VkImageLayout new_layout);

    VkMemoryBarrier memory_barrier(VkAccessFlags src_access_mask = 0, VkAccessFlags dst_access_mask = 0);

    VkImageMemoryBarrier image_memory_barrier(VkImage &image, VkImageLayout old_layout, VkImageLayout new_layout, VkAccessFlags src_access_mask = 0, VkAccessFlags dst_access_mask = 0);

    VkBufferMemoryBarrier buffer_memory_barrier(VkBuffer& buffer, VkAccessFlags src_access_mask = 0, VkAccessFlags dst_access_mask = 0, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);

    void execute_buffer_pipeline_barrier(VkCommandBuffer& cmd_buffer, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkBufferMemoryBarrier& buffer_barrier);

    void execute_image_pipeline_barrier(VkCommandBuffer& cmd_buffer, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkImageMemoryBarrier& img_barrier);

    void execure_memory_barrier(VkCommandBuffer& cmd_buffer, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkMemoryBarrier& memory_barrier);
}