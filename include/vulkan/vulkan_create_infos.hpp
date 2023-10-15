#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/vulkan/structs.hpp"

#include <string>
#include <vector>
#include <set>

namespace vulkan {
    inline VkInstanceCreateInfo instance_create_info(VkApplicationInfo *app, 
                                              const std::vector<const char*> &val_layer, 
                                              const std::vector<const char*> &extensions)
    {
        return {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = app,
            .enabledLayerCount = (uint32_t)val_layer.size(),
            .ppEnabledLayerNames = val_layer.data(),
            .enabledExtensionCount = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = extensions.data()
        };
    }
    
    inline VkDeviceCreateInfo logical_device_create_info(VkPhysicalDeviceFeatures *features,
                                                const std::vector<VkDeviceQueueCreateInfo> &queue_infos,
                                                const std::vector<const char*> &extensions)
    {
        return {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pQueueCreateInfos = queue_infos.data(),
            .queueCreateInfoCount = (uint32_t)queue_infos.size(),
            .pEnabledFeatures = features,
            .enabledExtensionCount = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = extensions.data()
        };
    }

    inline VkSwapchainCreateInfoKHR swap_chain_create_info(GLFWwindow *window, 
                                                    uint32_t min_image_count,
                                                    swap_chain_support_details swap_chain_support,
                                                    VkSurfaceFormatKHR format,
                                                    VkExtent2D extent,
                                                    VkPresentModeKHR present_mode,
                                                    VkSurfaceKHR &surface, 
                                                    std::vector<uint32_t> queue_family_indicies,
                                                    VkSwapchainKHR old_swap_chain = VK_NULL_HANDLE)
    {
        bool queue_flag = queue_family_indicies[0] != queue_family_indicies[1];
        
        return {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = surface,
            .minImageCount = min_image_count,
            .imageFormat = format.format,
            .imageColorSpace = format.colorSpace,
            .imageExtent = extent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = queue_flag ? (uint32_t)queue_family_indicies.size() : 0,
            .pQueueFamilyIndices = queue_flag ? (uint32_t*)&queue_family_indicies : nullptr,
            .preTransform = swap_chain_support.capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = present_mode,
            .clipped = VK_TRUE,
            .oldSwapchain = old_swap_chain
        };
    }

    inline VkImageCreateInfo image_create_info(VkFormat &image_format, VkImageUsageFlags usage, VkExtent3D &extent) {
        return VkImageCreateInfo {      
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = image_format,
            .extent = extent,
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = usage
        };
    }

    inline VkImageViewCreateInfo image_view_create_info(VkImage &image, 
                                                 VkFormat format,
                                                 VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT,
                                                 VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D, 
                                                 uint32_t layers = 1, 
                                                 uint32_t levels = 1)
    {
        return VkImageViewCreateInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = view_type,
            .format = format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = aspect_flags,
                .baseMipLevel = 0,
                .levelCount = levels,
                .baseArrayLayer = 0,
                .layerCount = layers
            },
        };
    }

    inline VkFramebufferCreateInfo framebuffer_create_info(VkImageView *image_view, 
                                                    VkRenderPass &render_pass,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint32_t layers = 1)
    {
        return VkFramebufferCreateInfo {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .width = width,
            .height = height,
            .layers = layers,
            .pAttachments = image_view
        };
    }

    inline VkCommandPoolCreateInfo command_pool_create_info(uint32_t queue_fam_index, 
                                                            VkCommandPoolCreateFlags flags)
    {
        return VkCommandPoolCreateInfo {
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            nullptr,
            flags,
            queue_fam_index
        };
    };

    inline VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool &pool, 
                                                                    VkCommandBufferLevel level, 
                                                                    uint32_t count = 1)
    {
        return VkCommandBufferAllocateInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = pool,
            .level = level,
            .commandBufferCount = count
        };
    };

    inline VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info(VkDescriptorSetLayoutBinding *binding, uint32_t binding_count) {
        return VkDescriptorSetLayoutCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .bindingCount = binding_count,
            .pBindings = binding
        };
    };

    inline VkDescriptorSetAllocateInfo descriptor_set_allocate_info(VkDescriptorPool &pool, VkDescriptorSetLayout *layout) {
        return VkDescriptorSetAllocateInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = pool,
            .descriptorSetCount = 1,
            .pSetLayouts = layout
        };
    };

    inline VkDescriptorPoolCreateInfo descriptor_pool_create_info(VkDescriptorPoolSize *pool_sizes, uint32_t size, uint32_t max_sets) {
        return VkDescriptorPoolCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .poolSizeCount = size,
            .pPoolSizes = pool_sizes,
            .maxSets = max_sets
        };
    };

    inline VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags) {
        return VkFenceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = flags
        };
    }

    inline VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags) {
        return VkSemaphoreCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = flags
        };
    }

    inline VkRenderPassCreateInfo render_pass_create_info(VkAttachmentDescription *attachment, uint32_t attachment_count, 
                                                          VkSubpassDescription *subpass, uint32_t subpass_count, 
                                                          VkSubpassDependency *dependency, uint32_t dependency_count) {
        return VkRenderPassCreateInfo {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .attachmentCount = attachment_count,
            .pAttachments = attachment,
            .subpassCount = subpass_count,
            .pSubpasses = subpass,
            .dependencyCount = dependency_count,
            .pDependencies = dependency
        };
    }

    inline VkPipelineLayoutCreateInfo pipeline_layout_create_info(VkDescriptorSetLayout *set_layout, uint32_t set_layout_count,
                                                                  VkPushConstantRange *push_constant_range, uint32_t push_constant_range_count) 
    {
        return VkPipelineLayoutCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .setLayoutCount = set_layout_count,
            .pSetLayouts = set_layout,
            .pushConstantRangeCount = push_constant_range_count,
            .pPushConstantRanges = push_constant_range
        };   
    }

    inline VkComputePipelineCreateInfo compute_pipeline_create_info(VkPipelineLayout &pipeline_layout, VkPipelineShaderStageCreateInfo &stage_create_info) {
        return VkComputePipelineCreateInfo {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .stage = stage_create_info,
            .layout = pipeline_layout,
            .flags = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1
        };
    }

    inline VkShaderModuleCreateInfo shader_module_create_info(uint32_t size, const char* data) {
        return VkShaderModuleCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .codeSize = size,
            .pCode = reinterpret_cast<const uint32_t*>(data)
        };
    }
}
