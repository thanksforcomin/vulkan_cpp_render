#include "include/vulkan/vulkan_destructors.hpp"

namespace vulkan {
    void delete_instance(VkInstance &inst) {
        vkDestroyInstance(inst, nullptr);
    }

    void delete_device(VkDevice &dev) {
        vkDestroyDevice(dev, nullptr);
    }

    void delete_swap_chain(VkDevice &dev, VkSwapchainKHR &swap_chain) {
        vkDestroySwapchainKHR(dev, swap_chain, nullptr);
    }
}