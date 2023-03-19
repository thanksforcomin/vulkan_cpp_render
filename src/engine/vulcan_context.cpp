#include "include/engine/vulkan_context.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>

VulkanContext::VulkanContext() : 
    instance(init_vulkan()),
    window(),
    phys_device(load_phys_device()),
    device(load_logical_device(phys_device))
{
    std::cout << "new vulkan context\n";
    poll_main_loop();
}

VulkanContext::~VulkanContext() {
    vkDestroyInstance(instance, nullptr);
}

VkInstance VulkanContext::init_vulkan() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // what extensions we need to use vulkan

    VkApplicationInfo appInfo; // general info
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
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
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

    VkDeviceQueueCreateInfo queue_create_info = {}; //device rendering queue create info
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = indicies.graphics_family.value();
    queue_create_info.queueCount = 1;
    float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkDeviceCreateInfo create_info = {}; //logical device create info
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.queueCreateInfoCount = 1;

    VkPhysicalDeviceFeatures device_fetures = {};
    create_info.pEnabledFeatures = &device_fetures;
    create_info.enabledExtensionCount = 0;
    create_info.enabledLayerCount = 0;

    //i have no idea what im doing please help
    VkDevice log_dev;
    VkResult res = vkCreateDevice(dev, &create_info, nullptr, &log_dev);
    if (res == VK_SUCCESS) {
        vkGetDeviceQueue(log_dev, indicies.graphics_family.value(), 0, &queue);
        std::cout << "logical device created\n";
        return log_dev;
    }
    else {
        std::runtime_error("failed to create device\n");
    }
}

bool VulkanContext::is_device_suitable(VkPhysicalDevice dev) {
    QueueFamilyIndicies indicies = find_queue_family(dev);

    return indicies.is_complete();
}

VulkanContext::QueueFamilyIndicies VulkanContext::find_queue_family(VkPhysicalDevice dev) {
    QueueFamilyIndicies ind;

    unsigned int queue_families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_families_count, nullptr);

    std::vector<VkQueueFamilyProperties> available_queue_families(queue_families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_families_count, available_queue_families.data());

    int count = 0;
    for (const auto &queue_family : available_queue_families) {
        if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            ind.graphics_family = count;
            count++;
        }
        if(ind.is_complete())
            break;
    }

    return ind;
}