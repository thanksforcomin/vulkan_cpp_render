#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader.hpp"
#include "include/engine/frame.hpp"
#include "include/vulkan/vulkan_utils.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
#include <cstring>

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

//#include <boost/log/trivial.hpp>

/*TODO:
Redesign Window class to be static, design the queue_family to be in two separate files*/

namespace engine
{
    VulkanContext::VulkanContext(std::vector<const char*> extensions, std::vector<const char*> device_extensions) : 
        game_window(glfw::create_window(720, 720)),
        instance(vulkan::create_instance("minivan", extensions)),
        surface(vulkan::create_surface(instance, game_window)),
        device(load_device(device_extensions)),
        queue_family(vulkan::find_queue_family(device.physical, surface)),
        graphics_queue(vulkan::create_queue(device.logical, queue_family.graphics_family.value())),
        present_queue(vulkan::create_queue(device.logical, queue_family.present_family.value())),
        swap_chain(this)
    {
        create_allocator();
        std::cout << "new vulkan context\n";
    }

    VulkanContext::~VulkanContext()
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyDevice(device.logical, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    vulkan::vulkan_device VulkanContext::load_device(std::vector<const char*> device_extensions)
    {
        vulkan::vulkan_device dev;
        dev.physical = vulkan::create_physical_device(instance, surface, device_extensions);
        dev.logical = vulkan::create_logical_device(dev.physical, surface, device_extensions);
        std::cout << "device created\n";
        return dev;
    }

    void VulkanContext::set_debug_messenger()
    {
        VkDebugUtilsMessengerCreateInfoEXT create_info;
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = debug_callback;
        create_info.pUserData = nullptr;

        if (create_debug_messenger(&create_info, nullptr, &debug_messenger) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to load debug layer");
        }
    }

    void VulkanContext::create_allocator() {
        VmaAllocatorCreateInfo create_info{};
        create_info.physicalDevice = device.physical;
        create_info.device = device.logical;
        create_info.instance = instance;
        if(vmaCreateAllocator(&create_info, &allocator) != VK_SUCCESS) throw std::runtime_error("cannot create VMA allocator");
        else std::cout << "created VMA allocator\n";
    }

    VkResult VulkanContext::create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT *create_info,
                                                   const VkAllocationCallbacks *allocator,
                                                   VkDebugUtilsMessengerEXT *messenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, create_info, allocator, messenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanContext::submit(const Frame *fr)
    {
        vulkan::submit_frame(
            {fr->command_buffer.command_buffer}, 
            {fr->present_semaphore}, 
            {fr->graphics_semaphore}, 
            fr->fence, 
            graphics_queue, 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    }

    void VulkanContext::present(const Frame *fr, uint32_t *index)
    {
        vulkan::present_frame(swap_chain.swap_chain, {fr->graphics_semaphore}, index, graphics_queue);
    }

    // STATIC VARIABLES
    std::vector<const char *> VulkanContext::required_device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    std::vector<const char *> VulkanContext::validation_layers{"VK_LAYER_KHRONOS_validation"};
    VkPipelineStageFlags VulkanContext::wait_flag = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
        VkDebugUtilsMessageSeverityFlagsEXT msg_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void *user_data)
    {
        std::cerr << "validation layers: " << callback_data->pMessage << std::endl;

        return VK_FALSE;
    }
}