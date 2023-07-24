#include "include/engine/vulkan_utils.hpp"

namespace engine {
    namespace vulkan {
        bool is_device_valid(VkPhysicalDevice &dev, VkSurfaceKHR &surface) {
            queue_family_indicies indicies = find_queue_family(dev, surface);

            bool supports_extensions = device_extensions_support(dev);
            bool swap_chain_suitable = 0;
            if (supports_extensions)
            {
                swap_chain_support_details swap_chain_support = get_swap_chain_support(dev);
                swap_chain_suitable = (!swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty());
            }

            return indicies.is_complete() && supports_extensions && swap_chain_suitable;
        }

        swap_chain_support_details swap_chain_support(VkPhysicalDevice &dev) {

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
    }
}