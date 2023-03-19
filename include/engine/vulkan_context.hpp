#pragma once

#include "include/engine/window.hpp"

#include <string>
#include <vector>
#include <optional>

class VulkanContext {
    private:
        struct QueueFamilyIndicies {
            std::optional<uint32_t> graphics_family;
            bool is_complete() { return graphics_family.has_value(); }
        };

    public:
        VulkanContext();
        ~VulkanContext();

    private:
        void poll_main_loop();
        void clean();

        VkPhysicalDevice load_phys_device();
        VkDevice load_logical_device(VkPhysicalDevice& dev);
        VkInstance init_vulkan();

        Window window;
        VkInstance instance;
        VkPhysicalDevice phys_device;
        VkDevice device;
        VkQueue queue;

        QueueFamilyIndicies find_queue_family(VkPhysicalDevice dev);
        bool is_device_suitable(VkPhysicalDevice dev);
};