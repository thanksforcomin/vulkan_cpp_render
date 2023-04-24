#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>

/*TODO:
Redesign Window class to be static, design the queue_family to be in two separate files*/

namespace engine {
    VulkanContext::VulkanContext() : 
        instance(init_vulkan()),
        window(instance),
        surface(load_surface()),
        device(load_device()),
        queue(load_device_queue(device, queue_families::GRAPHICS)),
        present_queue(load_device_queue(device, queue_families::PRESENT)),
        swap_chain(this),
        render_pass(init_render_pass())
    {   
        std::cout << "new vulkan context\n";
        poll_main_loop();
    }

    VulkanContext::~VulkanContext() {
        //swap_chain.~SwapChain();
        //vkDestroySurfaceKHR(instance, surface, nullptr);
        //vkDestroyDevice(device.logical, nullptr);
        //vkDestroyInstance(instance, nullptr);
    }

    VkInstance VulkanContext::init_vulkan() {
        std::vector<const char *> required_extensions = get_required_extensions();

        VkApplicationInfo appInfo{}; // general info
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = APP_NAME;
        appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
        appInfo.pEngineName = ENGINE_NAME;
        appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
        appInfo.apiVersion = VK_API_VERSION_1_0;

        std::cout << ENGINE_NAME << " "
                << " 1, 0, 0 \n";

        VkInstanceCreateInfo createInfo{}; // like a config to a ulkan context
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = (uint32_t)required_extensions.size();
        createInfo.ppEnabledExtensionNames = required_extensions.data();
        createInfo.enabledLayerCount = 0;

        VkInstance inst;
        VkResult res = vkCreateInstance(&createInfo, nullptr, &inst);

        if(res != VK_SUCCESS) {
            throw std::runtime_error("something went wrong");
        } else {
            std::cout << "vulkan content is ready\n";
        }

        return inst;
    }

    void VulkanContext::poll_main_loop() {
        while(window.is_alive()) {
            glfwPollEvents();
        }
    }

    VkPhysicalDevice VulkanContext::load_phys_device() {
        unsigned int device_count = 0;
        vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

        if(device_count == 0)
            std::runtime_error("no available devices, closing");

        std::vector<VkPhysicalDevice> available_devices(device_count);
        vkEnumeratePhysicalDevices(instance, &device_count, available_devices.data());

        for(const auto& dev : available_devices) {
            if(is_device_suitable(dev)) {
                VkPhysicalDeviceProperties device_properties;
                vkGetPhysicalDeviceProperties(dev, &device_properties);
                std::cout << device_properties.deviceName << " was chosen as the working device\n";
                return dev;
            }
        }

        std::runtime_error("no device is good enough.");
        return VK_NULL_HANDLE;
    }

    VkDevice VulkanContext::load_logical_device(VkPhysicalDevice& dev) {
        QueueFamilyIndicies indicies = find_queue_family(dev);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> unique_family_queues = {indicies.graphics_family.value(), indicies.present_family.value()};
        float queue_priority = 1.0f;

        for(uint32_t queue_family : unique_family_queues) {
            VkDeviceQueueCreateInfo queue_create_info = {};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueCount = 1;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkDeviceCreateInfo create_info = {}; //logical device create info
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.queueCreateInfoCount = (uint32_t)queue_create_infos.size();

        VkPhysicalDeviceFeatures device_fetures = {};
        create_info.pEnabledFeatures = &device_fetures;
        create_info.enabledExtensionCount = (uint32_t)required_device_extensions.size();
        create_info.ppEnabledExtensionNames = required_device_extensions.data();
        create_info.enabledLayerCount = 0;

        //i have no idea what im doing please help
        VkDevice log_dev;
        if (vkCreateDevice(dev, &create_info, nullptr, &log_dev) == VK_SUCCESS) {
            //vkGetDeviceQueue(log_dev, indicies.graphics_family.value(), 0, &queue);
            std::cout << "logical device created\n";
            return log_dev;
        }
        else {
            return VK_NULL_HANDLE;
            std::runtime_error("failed to create device\n");
        }
    }

    VkSurfaceKHR VulkanContext::load_surface() {
        VkSurfaceKHR res;
        if(glfwCreateWindowSurface(instance, window.window_ptr, nullptr, &res) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface\n");
        } else {
            return res;
        }
    }

    vulkan_device VulkanContext::load_device() {
        vulkan_device dev;
        dev.physical = load_phys_device();
        dev.logical = load_logical_device(dev.physical);
        std::cout << "device created\n";
        return dev;
    }

    VkQueue VulkanContext::load_device_queue(const vulkan_device& dev, queue_families family) {
        VkQueue res;
        QueueFamilyIndicies indicies = find_queue_family(dev.physical);
        
        if(family == queue_families::GRAPHICS)
            vkGetDeviceQueue(dev.logical, indicies.graphics_family.value(), 0, &res);
        else if(family == queue_families::PRESENT)
            vkGetDeviceQueue(dev.logical, indicies.present_family.value(), 0, &res);
        std::cout << "device queue created\n";
        return res;
    }

    bool VulkanContext::is_device_suitable(VkPhysicalDevice dev) {
        QueueFamilyIndicies indicies = find_queue_family(dev);

        bool supports_extensions = device_extension_support(dev);
        bool swap_chain_suitable = 0;
        if(supports_extensions) {
            swap_chain_support_details swap_chain_support = query_swap_chain_support(dev);
            swap_chain_suitable = (!swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty());
        }

        return indicies.is_complete() && supports_extensions && swap_chain_suitable;
    }

    QueueFamilyIndicies VulkanContext::find_queue_family(const VkPhysicalDevice &dev) const {
        QueueFamilyIndicies ind;

        unsigned int queue_families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_families_count, nullptr);

        std::vector<VkQueueFamilyProperties> available_queue_families(queue_families_count);
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_families_count, available_queue_families.data());

        int count = 0;
        for (const auto &queue_family : available_queue_families) {
            VkBool32 present_support = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(dev, count, surface, &present_support);

            if(present_support)
                ind.present_family = count;
            if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                ind.graphics_family = count;
            if(ind.is_complete())
                break;
            count++;
        }

        return ind;
    }

    bool VulkanContext::device_extension_support(VkPhysicalDevice dev) {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(required_device_extensions.begin(), required_device_extensions.end());

        for (const auto& extension : available_extensions) {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    swap_chain_support_details VulkanContext::query_swap_chain_support(const VkPhysicalDevice &dev) {
        swap_chain_support_details details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &details.capabilities);
        
        //query for surface formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, nullptr);
        if(format_count) {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, details.formats.data());
        }

        //query for presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &presentModeCount, nullptr);

        if (presentModeCount) {
            details.present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &presentModeCount, details.present_modes.data());
        }

        return details;
    }

    swap_chain_support_details VulkanContext::query_swap_chain_support() const { //because it may be useful
        swap_chain_support_details details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.physical, surface, &details.capabilities);
        
        //query for surface formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.physical, surface, &format_count, nullptr);
        if(format_count) {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device.physical, surface, &format_count, details.formats.data());
        }

        //query for presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device.physical, surface, &presentModeCount, nullptr);

        if (presentModeCount) {
            details.present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device.physical, surface, &presentModeCount, details.present_modes.data());
        }

        return details;
    }

    std::vector<const char*> VulkanContext::get_required_extensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        std::cout << "Extensions: " << glfwExtensions << "\n";

        return extensions;
    }

    //STATIC VARIABLES
    std::vector<const char*> VulkanContext::required_device_extensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}