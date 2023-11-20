#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"

#include <vector>
#include <string>

namespace glfw {
    GLFWwindow* create_window(uint32_t height, uint32_t width);
}

namespace vulkan {
    VkSurfaceKHR create_surface(VkInstance &inst, GLFWwindow *window);

    VkInstance create_instance(std::string application_name, std::vector<const char*> extensions = {});

    VkPhysicalDevice create_physical_device(VkInstance &inst, VkSurfaceKHR &surface, std::vector<const char*>& device_extensions);

    VkDevice create_logical_device(VkPhysicalDevice &dev, VkSurfaceKHR &surface, std::vector<const char*>& device_extensions);

    VkQueue create_queue(VkDevice &dev, uint32_t queue_family_index);
    
    VkSwapchainKHR create_swap_chain(vulkan_device &vulkan_dev, VkSurfaceKHR &surface, queue_family_indicies indicies, swap_chain_support_details support_details, 
                                     GLFWwindow *window, VkSurfaceFormatKHR format, VkExtent2D extent, VkPresentModeKHR present_mode);
                                    
    VkImage create_image(VkDevice &dev, VkFormat format, VkImageUsageFlags usage, VkExtent3D &extent);

    VkImageView create_image_view(VkDevice &dev, VkImage &image, VkFormat &format);

    VkFramebuffer create_framebuffer(VkDevice &dev, VkRenderPass &render_pass, VkImageView *image_attachment, VkExtent2D extent, uint32_t attachment_count = 1);

    VkCommandPool create_command_pool(VkDevice &dev, VkCommandPoolCreateFlags flags, uint32_t queue_family_index);

    VkCommandBuffer allocate_command_buffer(VkDevice &dev, VkCommandPool &command_pool, VkCommandBufferLevel level, uint32_t count = 1);

    VkDescriptorSetLayout create_descriptor_set_layout(VkDevice &dev, std::vector<VkDescriptorSetLayoutBinding> bindings);

    VkDescriptorSet allocate_descriptor_set(VkDevice &dev, VkDescriptorPool &pool, VkDescriptorSetLayout &layout);

    VkDescriptorPool create_descriptor_pool(VkDevice &dev, std::vector<VkDescriptorPoolSize> pool_sizes, uint32_t max_sets);

    VkFence create_fence(VkDevice &dev, VkFenceCreateFlags flags = VK_FENCE_CREATE_SIGNALED_BIT);

    VkSemaphore create_semaphore(VkDevice &dev, VkSemaphoreCreateFlags flags = 0);

    VkRenderPass create_render_pass(VkDevice &dev, std::vector<VkAttachmentDescription> attachments, 
                                    std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies = {});

    VkPipelineLayout create_pipeline_layout(VkDevice &dev, std::vector<VkDescriptorSetLayout> set_layouts, std::vector<VkPushConstantRange> push_constants = {});

    VkPipeline create_compute_pipeline(VkDevice &dev, VkPipelineLayout &pipeline_layout, VkPipelineShaderStageCreateInfo &compute_shader);

    VkShaderModule create_chader_module(VkDevice &dev, std::string&& data);
}
