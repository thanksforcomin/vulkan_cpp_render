#pragma once

#include "include/engine/window.hpp"
#include "include/engine/details.hpp"
#include "include/engine/swap_chain.hpp"

#include <string>
#include <vector>
#include <optional>

namespace engine {
    class VulkanContext {
        private:
            static std::vector<const char*> required_device_extensions;
        public:
            friend class SwapChain;

            VulkanContext();
            ~VulkanContext();

        private:
            void poll_main_loop();

            VkPhysicalDevice load_phys_device();
            VkDevice load_logical_device(VkPhysicalDevice& dev);
            VkSurfaceKHR load_surface();
            vulkan_device load_device();
            VkQueue load_device_queue(const vulkan_device& dev, queue_families family = queue_families::GRAPHICS);
            VkInstance init_vulkan();

            //window first (we need extensions)
            Window window;
            //Vulkan instance second
            VkInstance instance;
            //Surface then
            VkSurfaceKHR surface;
            // Physical and logical devices
            vulkan_device device;
            //Queues 
            VkQueue queue;
            VkQueue present_queue;
            //swap chain
            SwapChain swap_chain;

            QueueFamilyIndicies find_queue_family(const VkPhysicalDevice &dev);
            swap_chain_support_details query_swap_chain_support(const VkPhysicalDevice &dev);
            swap_chain_support_details query_swap_chain_support(); //for the default single device of the class
            bool is_device_suitable(VkPhysicalDevice dev);
            bool device_extension_support(VkPhysicalDevice dev);
            std::vector<const char *> get_required_extensions();
    };
}