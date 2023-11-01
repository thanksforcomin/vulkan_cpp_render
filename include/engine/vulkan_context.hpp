#pragma once

#include "include/engine/swap_chain.hpp"

#include "include/vulkan/vulkan_initializers.hpp"

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

            VulkanContext(std::vector<const char*> extensions, std::vector<const char*> device_extensions);
            ~VulkanContext();
            
            void submit(const Frame *fr);
            void present(const Frame *fr, uint32_t *index);

        private:
            vulkan::vulkan_device load_device(std::vector<const char*> device_extensions);
            void set_debug_messenger();
            void create_allocator();

        public: //i have no possible idea how to make it better
            //window first (we need extensions)
            GLFWwindow* game_window;
            //Vulkan instance second
            VkInstance instance;
            //Surface then
            VkSurfaceKHR surface;
            // Physical and logical devices
            vulkan::vulkan_device device;
            //family indicies
            vulkan::queue_family_indicies queue_family;
            //Queues 
            const VkQueue graphics_queue;
            const VkQueue present_queue;
            // swap chain
            SwapChain swap_chain;
            // memory allocator
            VmaAllocator allocator;

        private:
            VkResult create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT* create_info,
                                            const VkAllocationCallbacks* allocator, 
                                            VkDebugUtilsMessengerEXT* messenger);
            std::vector<const char *> get_required_extensions();

            VkDebugUtilsMessengerEXT debug_messenger;
    };
}