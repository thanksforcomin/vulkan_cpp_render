#pragma once

#include "include/engine/swap_chain.hpp"

#include "vk_mem_alloc.h"

#include <string>
#include <vector>
#include <optional>

namespace engine {
    class Frame; //forward decl

    class VulkanContext {
        private:
            static std::vector<const char*> required_device_extensions;
            static std::vector<const char*> validation_layers;

            static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
                VkDebugUtilsMessageSeverityFlagsEXT msg_type,
                const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                void* user_data
            );

            static VkPipelineStageFlags wait_flag;
        public:
            friend class SwapChain;
            friend class Shader;

            VulkanContext();
            ~VulkanContext();

            QueueFamilyIndicies find_queue_family(const VkPhysicalDevice &dev) const;
            void submit(const Frame *fr);
            void present(const Frame *fr, uint32_t *index);

        private:

            VkPhysicalDevice load_phys_device();
            VkDevice load_logical_device(VkPhysicalDevice& dev);
            VkSurfaceKHR load_surface();
            vulkan_device load_device();
            VkQueue load_device_queue(const vulkan_device& dev, queue_families family = queue_families::GRAPHICS);
            VkInstance init_vulkan();
            void set_debug_messenger();
            void create_allocator();

        public: //i have no possible idea how to make it better
            //window first (we need extensions)
            Window window;
            //Vulkan instance second
            VkInstance instance;
            //Surface then
            VkSurfaceKHR surface;
            // Physical and logical devices
            const vulkan_device device;
            //Queues 
            const VkQueue graphics_queue;
            const VkQueue present_queue;
            // swap chain
            SwapChain swap_chain;
            // memory allocator
            VmaAllocator allocator;

        private:
            swap_chain_support_details query_swap_chain_support(const VkPhysicalDevice &dev);
            swap_chain_support_details query_swap_chain_support() const; //for the default single device of the class
            bool is_device_suitable(VkPhysicalDevice dev);
            bool device_extension_support(VkPhysicalDevice dev);
            bool check_validation_layers_support();
            VkResult create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT* create_info,
                                            const VkAllocationCallbacks* allocator, 
                                            VkDebugUtilsMessengerEXT* messenger);
            std::vector<const char *> get_required_extensions();

            VkDebugUtilsMessengerEXT debug_messenger;
    };
}