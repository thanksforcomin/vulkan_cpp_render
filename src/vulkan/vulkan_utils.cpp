#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_create_infos.hpp"

#include <set>
#include <string>
#include <cstring>
#include <limits>
#include <algorithm>
#include <iostream>

namespace vulkan {
    /**
     * Retrieves the required extensions for the application.
     *
     * return A vector of const char* containing the required extensions.
     */
    std::vector<const char*> require_extensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        return {glfwExtensions, glfwExtensions + glfwExtensionCount};
    }

    bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface, std::vector<const char*>& device_extensions) {
        queue_family_indicies indicies = find_queue_family(dev, surface);

        bool supports_extensions = device_extensions_support(dev, device_extensions);
        bool swap_chain_suitable = 0;
        if (supports_extensions)
        {
            swap_chain_support_details swap_chain_support = get_swap_chain_support(dev, surface);
            swap_chain_suitable = (!swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty());
        }

        return indicies.is_complete() && supports_extensions && swap_chain_suitable;
    }

    bool device_extensions_support(VkPhysicalDevice &dev, std::vector<const char*>& extensions) {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(extensions.begin(), extensions.end());

        for (const auto &extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    bool validation_layers_support(std::vector<const char *> validation_layers) {
        uint32_t layers_count;
        vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layers_count);
        vkEnumerateInstanceLayerProperties(&layers_count, &available_layers[0]);

        for (const char *layer_name : validation_layers)
        {
            bool layer_found = false;
            for (const auto &layer_properties : available_layers)
            {
                if (std::strcmp(layer_name, layer_properties.layerName) == 0)
                {
                    layer_found = true;
                    break;
                }
            }

            if (!layer_found)
                return false;
        }

        return true;
    }

    swap_chain_support_details get_swap_chain_support(VkPhysicalDevice &dev, VkSurfaceKHR &surface) {
        swap_chain_support_details details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &details.capabilities);

        // query for surface formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, nullptr);
        if (format_count) {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, details.formats.data());
        }

        // query for presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &presentModeCount, nullptr);

        if (presentModeCount) {
            details.present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &presentModeCount, details.present_modes.data());
        }

        return details;
    }

    queue_family_indicies find_queue_family(VkPhysicalDevice &dev, VkSurfaceKHR &surface) {
        queue_family_indicies ind;

        unsigned int queue_families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_families_count, nullptr);

        std::vector<VkQueueFamilyProperties> available_queue_families(queue_families_count);
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_families_count, available_queue_families.data());

        int count = 0;
        for (const auto &queue_family : available_queue_families)
        {
            VkBool32 present_support = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(dev, count, surface, &present_support);

            if (present_support)
                ind.present_family = count;
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                ind.graphics_family = count;
            if (ind.is_complete())
                break;
            count++;
        }

        return ind;
    }

    VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &available_present_modes) {
        for (const auto& available_present_mode : available_present_modes) {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return available_present_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;
        else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            //std::cout << "framebuffer size: " << width << " x " << height << "\n";

            VkExtent2D new_extent = {
                (uint32_t)width,
                (uint32_t)height
            };

            new_extent.width = std::clamp(new_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            new_extent.height = std::clamp(new_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return new_extent;
        }
    }

    VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats) {
        for (const auto& available_format : available_formats) {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return available_format;
            }
        }

        return available_formats[0];
    }

    VkFormat find_format(VkPhysicalDevice &dev, std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(dev, format, &properties);

            if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("couldn't find a matching format");
    }

    VkFormat find_depth_format(VkPhysicalDevice &dev) {
        return find_format(
            dev,
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkExtent3D extent_2d_to_3d(VkExtent2D extent) {
        return VkExtent3D {
            .width = extent.width,
            .height = extent.height,
            .depth = 1
        };
    }

    std::vector<VkImage> get_swap_chain_images(VkDevice &dev, VkSwapchainKHR &swap_chain) {
        uint32_t image_count;
        vkGetSwapchainImagesKHR(dev, swap_chain, &image_count, nullptr);
        std::vector<VkImage> swap_chain_images(image_count);
        vkGetSwapchainImagesKHR(dev, swap_chain, &image_count, swap_chain_images.data());
        return swap_chain_images;
    }

    VkCommandBufferBeginInfo get_command_buffer_begin_info(VkCommandBufferUsageFlags flags) {
        return VkCommandBufferBeginInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = flags,
            .pInheritanceInfo = nullptr
        };
    };

    VkRenderPassBeginInfo get_render_pass_begin_info(VkRenderPass &render_pass, VkRect2D render_area, VkClearValue *clear_value, uint32_t clear_value_size) {
        return VkRenderPassBeginInfo {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = render_pass,
            .renderArea = render_area,
            .clearValueCount = clear_value_size,
            .pClearValues = clear_value
        };
    };

    VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding_point) {
        return VkDescriptorSetLayoutBinding {
            .binding = binding_point,
            .descriptorCount = 1,
            .descriptorType = type,
            .stageFlags = (VkShaderStageFlags)shader_stage,
            .pImmutableSamplers = nullptr
        };
    };

    VkDescriptorBufferInfo get_buffer_info(VkBuffer &buffer, VkDeviceSize size, uint32_t offset) {
        return VkDescriptorBufferInfo {
            .buffer = buffer,
            .offset = offset,
            .range = size
        };
    };

    VkWriteDescriptorSet get_descriptor_write_info(VkDescriptorType type, VkDescriptorSet dst_set, uint32_t binding, VkDescriptorBufferInfo &buffer_info) {
        return {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = dst_set,
            .dstBinding = binding,
            .descriptorCount = 1,
            .pBufferInfo = &buffer_info
        };
    };

    pipeline::pipeline_builder begin_pipeline_builder(VkPipelineLayout &layout, uint32_t subpass) {
        pipeline::pipeline_builder builder;
        builder.create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        builder.create_info.basePipelineHandle = VK_NULL_HANDLE;
        builder.create_info.basePipelineIndex = -1;
        builder.create_info.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
        builder.create_info.renderPass = nullptr;
        builder.create_info.subpass = subpass;
        builder.create_info.layout = layout;
        return builder;
    };

    VkAttachmentDescription get_color_attachment(VkFormat &format, VkImageLayout fin_layout, VkImageLayout init_layout) {
        return VkAttachmentDescription {
            .format = format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = init_layout,
            .finalLayout = fin_layout
        };
    };

    VkAttachmentDescription get_depth_attachment(VkFormat &format, VkImageLayout fin_layout, VkImageLayout init_layout) {
        return VkAttachmentDescription {
            .format = format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = init_layout,
            .finalLayout = fin_layout
        };
    };

    VkSubpassDependency get_subpass_dependency(uint32_t dst_subpass) {
        return VkSubpassDependency {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = dst_subpass,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        };
    }

    VkViewport get_viewport(VkExtent2D &extent) {
        return VkViewport {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)extent.width,
            .height = (float)extent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
    }

    VkSubmitInfo get_submit_info(VkCommandBuffer &command_buffer,
                                 std::vector<VkSemaphore>& wait_semaphores,
                                 std::vector<VkSemaphore>& sig_semaphores,
                                 uint32_t& wait_stages) 
    {
        return VkSubmitInfo {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &command_buffer,
            .waitSemaphoreCount = (uint32_t)wait_semaphores.size(),
            .pWaitSemaphores = wait_semaphores.data(),
            .signalSemaphoreCount = (uint32_t)sig_semaphores.size(),
            .pSignalSemaphores = sig_semaphores.data(),
            .pWaitDstStageMask = &wait_stages
        };
    }

    VkSubmitInfo get_submit_info(VkCommandBuffer &command_buffer) {
        return VkSubmitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &command_buffer
        };
    }

    void submit_command_buffer(const VkQueue& queue, VkSubmitInfo* submit_info, VkFence& fence) {
        if(vkQueueSubmit(queue, 1, submit_info, fence) != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer!\n");
    }

    void submit_frame(std::vector<VkCommandBuffer>&& command_buffers,
                      std::vector<VkSemaphore>&& wait_semaphores,
                      std::vector<VkSemaphore>&& sig_semaphores,
                      const VkFence& fence,
                      const VkQueue& queue,
                      VkPipelineStageFlags stage_flag
                      )
    {
        VkSubmitInfo submit_info {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBufferCount = (uint32_t)command_buffers.size(),
            .pCommandBuffers = command_buffers.data(),
            .waitSemaphoreCount = (uint32_t)wait_semaphores.size(),
            .pWaitSemaphores = wait_semaphores.data(),
            .signalSemaphoreCount = (uint32_t)sig_semaphores.size(),
            .pSignalSemaphores = sig_semaphores.data(),
            .pWaitDstStageMask = &stage_flag
        };
        if(vkQueueSubmit(queue, 1, &submit_info, fence) != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer!\n");
    }

    void present_frame(const VkSwapchainKHR &swap_chain, std::vector<VkSemaphore>&& wait_semops, uint32_t *index, const VkQueue& queue) {
        VkPresentInfoKHR present_info{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = (uint32_t)wait_semops.size(),
            .pWaitSemaphores = wait_semops.data(),
            .swapchainCount = 1,
            .pSwapchains = &swap_chain,
            .pImageIndices = index
        };
        if(vkQueuePresentKHR(queue, &present_info) != VK_SUCCESS)
            throw std::runtime_error("failed to present frame");
    }

    allocated_buffer allocate_buffer(VmaAllocator &allocator, size_t size, VkBufferUsageFlags flags, VmaMemoryUsage usage) {
        allocated_buffer buffer {
            .size = size,
            .allocator = &allocator
        };

        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.pNext = nullptr;
        create_info.size = size;
        create_info.usage = flags;

        VmaAllocationCreateInfo allocation_info{};
        allocation_info.usage = usage;

        vmaCreateBuffer(allocator, &create_info, &allocation_info, &buffer.buffer, &buffer.allocation, nullptr);
        return buffer;
    }

    allocated_image allocate_image(VmaAllocator &allocator, VkExtent3D extent, VkFormat format, VkImageUsageFlags flags, VmaMemoryUsage usage) {
        allocated_image image {
            .allocator = &allocator
        };

        VkImageCreateInfo create_info{image_create_info(format, flags, extent)};
        VmaAllocationCreateInfo allocation_info{
            .usage = usage
        };

        vmaCreateImage(allocator, &create_info, &allocation_info, &image.image, &image.allocation, nullptr);
        return image;
    }

    void deallocate_buffer(allocated_buffer &&buffer) {
        vmaDestroyBuffer(*buffer.allocator, buffer.buffer, buffer.allocation);
    }

    void upload_to_buffer(allocated_buffer &buffer, vertex::Vertex* data, uint32_t size) {
        void* ptr;
        vmaMapMemory(*buffer.allocator, buffer.allocation, &ptr);
        memcpy(ptr, data, size);
        vmaUnmapMemory(*buffer.allocator, buffer.allocation);
    }

    VkRenderingInfoKHR get_rendering_info(VkRect2D rendering_area, 
                                          VkRenderingAttachmentInfoKHR *color_attachment, 
                                          VkRenderingAttachmentInfoKHR *depth_attachment, 
                                          VkRenderingAttachmentInfoKHR *stencil_attachment,
                                          uint32_t layer_count,
                                          uint32_t color_attachment_count)
    {
        return VkRenderingInfoKHR {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
            .pNext = nullptr,
            .renderArea = rendering_area,
            .layerCount = layer_count,
            .colorAttachmentCount = color_attachment_count,
            .pColorAttachments = color_attachment,
            .pDepthAttachment = depth_attachment,
            .pStencilAttachment = stencil_attachment
        };
    }

    VkRenderingAttachmentInfoKHR get_rendering_attachment_info(VkImageView &image_view, 
                                                               VkImageLayout image_layout,
                                                               VkClearValue clear_val, 
                                                               VkAttachmentLoadOp load_op,
                                                               VkAttachmentStoreOp store_op)
    {
        return VkRenderingAttachmentInfoKHR {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
            .pNext = nullptr,
            .imageView = image_view,
            .imageLayout = image_layout,
            .loadOp = load_op,
            .storeOp = store_op,
            .clearValue = clear_val
        };
    }

    VkPhysicalDeviceDynamicRenderingFeaturesKHR get_dynamic_rendering_features() {
        return VkPhysicalDeviceDynamicRenderingFeaturesKHR {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
            .dynamicRendering = VK_TRUE
        };
    }

    VkMemoryBarrier memory_barrier(VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask) {
        return VkMemoryBarrier {
            .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = src_access_mask,
            .dstAccessMask = dst_access_mask
        };
    }

    VkImageMemoryBarrier image_memory_barrier(VkImage &image, VkImageLayout old_layout, VkImageLayout new_layout, VkAccessFlags src_access_mask , VkAccessFlags dst_access_mask) {
        return VkImageMemoryBarrier {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = src_access_mask,
            .dstAccessMask = dst_access_mask,
            .oldLayout = old_layout,
            .newLayout = new_layout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = image,
            .subresourceRange = VkImageSubresourceRange {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
    }

    VkBufferMemoryBarrier buffer_memory_barrier(VkBuffer& buffer, VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask, VkDeviceSize offset, VkDeviceSize size) {
        return VkBufferMemoryBarrier {
            .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = src_access_mask,
            .dstAccessMask = dst_access_mask,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .buffer = buffer,
            .offset = 0,
            .size = VK_WHOLE_SIZE
        };
    }

    void execute_buffer_pipeline_barrier(VkCommandBuffer& cmd_buffer, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkBufferMemoryBarrier& buffer_barrier) {
        vkCmdPipelineBarrier(cmd_buffer, src_stage_mask, dst_stage_mask, 0, 0, nullptr, 1, &buffer_barrier, 0, nullptr);
    }

    void execute_image_pipeline_barrier(VkCommandBuffer& cmd_buffer, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkImageMemoryBarrier& img_barrier) {
        vkCmdPipelineBarrier(cmd_buffer, src_stage_mask, dst_stage_mask, 0, 0, nullptr, 0, nullptr, 1, &img_barrier);
    }

    void execure_memory_barrier(VkCommandBuffer& cmd_buffer, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkMemoryBarrier& memory_barrier) {
        vkCmdPipelineBarrier(cmd_buffer, src_stage_mask, dst_stage_mask, 0, 1, &memory_barrier, 0, nullptr, 0, nullptr);
    }

    VkBufferImageCopy get_buffer_image_copy_region(VkExtent3D extent) {
        return VkBufferImageCopy {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1
            },
            .imageOffset = {0, 0, 0},
            .imageExtent = extent
        };
    }

    VkBufferImageCopy get_buffer_image_copy_region(VkExtent3D extent, VkImageSubresourceLayers subresource) {
        return VkBufferImageCopy {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = subresource,
            .imageOffset = {0, 0, 0},
            .imageExtent = extent
        };
    }

    void execute_image_copy(VkCommandBuffer& cmd_buffer, VkBuffer& staging_buffer, VkImage& image, VkBufferImageCopy copy_region) {
        vkCmdCopyBufferToImage(cmd_buffer, staging_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
    }
}