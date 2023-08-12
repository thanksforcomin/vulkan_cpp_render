#include "include/vulkan/vulkan_create_infos.hpp"
#include "include/vulkan/vulkan_utils.hpp"

namespace vulkan {
    VkInstanceCreateInfo instance_create_info(VkApplicationInfo *app, 
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

    VkDeviceCreateInfo logical_device_create_info(VkPhysicalDeviceFeatures *features,
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

    VkSwapchainCreateInfoKHR swap_chain_create_info(GLFWwindow *window,
                                                    uint32_t min_image_count,
                                                    swap_chain_support_details swap_chain_support,
                                                    VkSurfaceKHR &surface, 
                                                    std::vector<uint32_t> queue_family_indicies,
                                                    VkSwapchainKHR old_swap_chain)
    {
        VkSurfaceFormatKHR surface_format = choose_format(swap_chain_support.formats);
        bool queue_flag = queue_family_indicies[0] != queue_family_indicies[1];
        
        return {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = surface,
            .minImageCount = min_image_count,
            .imageFormat = surface_format.format,
            .imageColorSpace = surface_format.colorSpace,
            .imageExtent = choose_extent(swap_chain_support.capabilities, window),
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = queue_flag ? (uint32_t)queue_family_indicies.size() : 0,
            .pQueueFamilyIndices = queue_flag ? (uint32_t*)&queue_family_indicies : nullptr,
            .preTransform = swap_chain_support.capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = choose_present_mode(swap_chain_support.present_modes),
            .clipped = VK_TRUE,
            .oldSwapchain = old_swap_chain
        };
    }

    VkImageViewCreateInfo image_view_create_info(VkImage &image, 
                                                 VkFormat format,
                                                 VkImageViewType view_type, 
                                                 uint32_t layers, 
                                                 uint32_t levels) 
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
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = levels,
                .baseArrayLayer = 0,
                .layerCount = layers
            }
        };
    }

    VkFramebufferCreateInfo framebuffer_create_info(VkImageView *image_view, 
                                                    VkRenderPass &render_pass,
                                                    uint32_t width,
                                                    uint32_t height,
                                                    uint32_t layers)
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
}