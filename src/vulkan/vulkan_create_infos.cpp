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
                                                    swap_chain_support_details &swap_chain_support,
                                                    VkSurfaceKHR &surface, 
                                                    std::set<uint32_t> &queue_family_indicies,
                                                    VkSwapchainKHR old_swap_chain)
    {
        VkSurfaceFormatKHR surface_format = choose_format(swap_chain_support.formats);
        
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
            .queueFamilyIndexCount = (uint32_t)queue_family_indicies.size(),
            .pQueueFamilyIndices = (uint32_t*)&queue_family_indicies,
            .preTransform = swap_chain_support.capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = choose_present_mode(swap_chain_support.present_modes),
            .clipped = VK_TRUE,
            .oldSwapchain = old_swap_chain
        };
    }
}