#pragma once

#include "include/vulkan/structs.hpp"

namespace vulkan {
    void delete_instance(VkInstance &inst);

    void delete_device(VkDevice &dev);

    void delete_swap_chain(VkDevice &dev, VkSwapchainKHR &swap_chain);
}