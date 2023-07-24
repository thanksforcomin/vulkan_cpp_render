#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader.hpp"
#include "include/engine/pipeline.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
#include <cstring>

//#include <boost/log/trivial.hpp>

/*TODO:
Redesign Window class to be static, design the queue_family to be in two separate files*/

namespace engine
{
    VulkanContext::VulkanContext() : window(),
                                     instance(init_vulkan()),
                                     surface(load_surface()),
                                     queue_family(find_queue_family(device.physical)),
                                     device(load_device()),
                                     graphics_queue(load_device_queue(device, queue_families::GRAPHICS)),
                                     present_queue(load_device_queue(device, queue_families::PRESENT)),
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

    VkInstance VulkanContext::init_vulkan()
    {
        std::vector<const char *> required_extensions = get_required_extensions();

        if (IS_DEBUG && !check_validation_layers_support())
        {
            throw std::runtime_error("validation layers are not available");
        }

        VkApplicationInfo appInfo{}; // general info
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = APP_NAME;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = ENGINE_NAME;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        std::cout << ENGINE_NAME << " "
                  << " 1, 0, 0 \n";

        VkInstanceCreateInfo createInfo{}; // like a config to a ulkan context
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = (uint32_t)required_extensions.size();
        createInfo.ppEnabledExtensionNames = required_extensions.data();

        if (IS_DEBUG)
        {
            createInfo.enabledLayerCount = (uint32_t)validation_layers.size();
            createInfo.ppEnabledLayerNames = &validation_layers[0];
        }
        else
            createInfo.enabledLayerCount = 0;

        VkInstance inst;
        VkResult res = vkCreateInstance(&createInfo, nullptr, &inst);

        if (res != VK_SUCCESS)
        {
            std::cout << res << "\n";
            throw std::runtime_error("something went wrong");
        }
        else
        {
            std::cout << "vulkan content is ready\n";
        }

        return inst;
    }

    VkPhysicalDevice VulkanContext::load_phys_device()
    {
        unsigned int device_count = 0;
        vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

        if (device_count == 0)
            std::runtime_error("no available devices, closing");

        std::vector<VkPhysicalDevice> available_devices(device_count);
        vkEnumeratePhysicalDevices(instance, &device_count, available_devices.data());

        for (const auto &dev : available_devices)
        {
            if (is_device_suitable(dev))
            {
                VkPhysicalDeviceProperties device_properties;
                vkGetPhysicalDeviceProperties(dev, &device_properties);
                std::cout << device_properties.deviceName << " was chosen as the working device\n";
                return dev;
            }
        }

        std::runtime_error("no device is good enough.");
        return VK_NULL_HANDLE;
    }

    VkDevice VulkanContext::load_logical_device(VkPhysicalDevice &dev)
    {
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> unique_family_queues = {queue_family.graphics_family.value(), queue_family.present_family.value()};
        float queue_priority = 1.0f;

        for (uint32_t queue_family : unique_family_queues)
        {
            VkDeviceQueueCreateInfo queue_create_info = {};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueCount = 1;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkDeviceCreateInfo create_info = {}; // logical device create info
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.queueCreateInfoCount = (uint32_t)queue_create_infos.size();

        VkPhysicalDeviceFeatures device_fetures = {};
        create_info.pEnabledFeatures = &device_fetures;
        create_info.enabledExtensionCount = (uint32_t)required_device_extensions.size();
        create_info.ppEnabledExtensionNames = required_device_extensions.data();
        create_info.enabledLayerCount = 0;

        // i have no idea what im doing please help
        VkDevice log_dev;
        if (vkCreateDevice(dev, &create_info, nullptr, &log_dev) == VK_SUCCESS)
        {
            // vkGetDeviceQueue(log_dev, indicies.graphics_family.value(), 0, &queue);
            std::cout << "logical device created\n";
            return log_dev;
        }
        else
        {
            return VK_NULL_HANDLE;
            std::runtime_error("failed to create device\n");
        }
    }

    VkSurfaceKHR VulkanContext::load_surface()
    {
        VkSurfaceKHR res;

        if (glfwCreateWindowSurface(instance, window.window_ptr.get(), nullptr, &res) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface\n");
        }
        else
        {
            return res;
        }
    }

    vulkan_device VulkanContext::load_device()
    {
        vulkan_device dev;
        dev.physical = load_phys_device();
        dev.logical = load_logical_device(dev.physical);
        std::cout << "device created\n";
        return dev;
    }

    VkQueue VulkanContext::load_device_queue(const vulkan_device &dev, queue_families family)
    {
        VkQueue res;
        QueueFamilyIndicies indicies = find_queue_family(dev.physical);

        if (family == queue_families::GRAPHICS)
            vkGetDeviceQueue(dev.logical, indicies.graphics_family.value(), 0, &res);
        else if (family == queue_families::PRESENT)
            vkGetDeviceQueue(dev.logical, indicies.present_family.value(), 0, &res);
        std::cout << "device queue created\n";
        return res;
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

    bool VulkanContext::is_device_suitable(VkPhysicalDevice dev)
    {
        QueueFamilyIndicies indicies = find_queue_family(dev);

        bool supports_extensions = device_extension_support(dev);
        bool swap_chain_suitable = 0;
        if (supports_extensions)
        {
            swap_chain_support_details swap_chain_support = query_swap_chain_support(dev);
            swap_chain_suitable = (!swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty());
        }

        return indicies.is_complete() && supports_extensions && swap_chain_suitable;
    }

    QueueFamilyIndicies VulkanContext::find_queue_family(const VkPhysicalDevice &dev) const
    {
        QueueFamilyIndicies ind;

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

    bool VulkanContext::device_extension_support(VkPhysicalDevice dev)
    {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(required_device_extensions.begin(), required_device_extensions.end());

        for (const auto &extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    bool VulkanContext::check_validation_layers_support()
    {
        uint32_t layers_count;
        vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layers_count);
        vkEnumerateInstanceLayerProperties(&layers_count, &available_layers[0]);

        for (const char *layer_name : validation_layers)
        {
            bool layer_found = false;
            for (const auto &layer_properties : available_layers)
            {
                if (strcmp(layer_name, layer_properties.layerName) == 0)
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

    swap_chain_support_details VulkanContext::query_swap_chain_support(const VkPhysicalDevice &dev)
    {
        swap_chain_support_details details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &details.capabilities);

        // query for surface formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, nullptr);
        if (format_count)
        {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, details.formats.data());
        }

        // query for presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &presentModeCount, nullptr);

        if (presentModeCount)
        {
            details.present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &presentModeCount, details.present_modes.data());
        }

        return details;
    }

    swap_chain_support_details VulkanContext::query_swap_chain_support() const
    { // because it may be useful
        swap_chain_support_details details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.physical, surface, &details.capabilities);

        // query for surface formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.physical, surface, &format_count, nullptr);
        if (format_count)
        {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device.physical, surface, &format_count, details.formats.data());
        }

        // query for presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device.physical, surface, &presentModeCount, nullptr);

        if (presentModeCount)
        {
            details.present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device.physical, surface, &presentModeCount, details.present_modes.data());
        }

        return details;
    }

    std::vector<const char *> VulkanContext::get_required_extensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        std::cout << "Extensions: " << glfwExtensions << "\n";

        return extensions;
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
        VkSubmitInfo submit{};
        submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.pNext = nullptr;
        
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	    submit.pWaitDstStageMask = &waitStage;

        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &fr->present_semaphore;

        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &fr->graphics_semaphore;

        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &fr->command_dispatcher.command_buffer;

        if (vkQueueSubmit(graphics_queue, 1, &submit, fr->fence) != VK_SUCCESS)
            throw std::runtime_error("cannot submit a frame");
    }

    void VulkanContext::present(const Frame *fr, uint32_t *index)
    {
        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pNext = nullptr;

        present_info.swapchainCount = 1;
        present_info.pSwapchains = &swap_chain.swap_chain;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &fr->graphics_semaphore;

        present_info.pImageIndices = index;

        if (vkQueuePresentKHR(graphics_queue, &present_info) != VK_SUCCESS)
            throw std::runtime_error("failed to present frame");
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