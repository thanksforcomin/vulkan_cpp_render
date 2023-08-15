#include "include/vulkan/vulkan_utils.hpp"

#include <set>
#include <string>
#include <cstring>
#include <limits>
#include <algorithm>

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

    bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface) {
        queue_family_indicies indicies = find_queue_family(dev, surface);

        bool supports_extensions = device_extensions_support(dev, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
        bool swap_chain_suitable = 0;
        if (supports_extensions)
        {
            swap_chain_support_details swap_chain_support = get_swap_chain_support(dev, surface);
            swap_chain_suitable = (!swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty());
        }

        return indicies.is_complete() && supports_extensions && swap_chain_suitable;
    }

    bool device_extensions_support(VkPhysicalDevice &dev, std::vector<const char*> extensions) {
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

    std::vector<VkImage> get_swap_chain_images(VkDevice &dev, VkSwapchainKHR &swap_chain) {
        uint32_t image_count;
        vkGetSwapchainImagesKHR(dev, swap_chain, &image_count, nullptr);
        std::vector<VkImage> swap_chain_images(image_count);
        vkGetSwapchainImagesKHR(dev, swap_chain, &image_count, swap_chain_images.data());
        return swap_chain_images;
    }
}