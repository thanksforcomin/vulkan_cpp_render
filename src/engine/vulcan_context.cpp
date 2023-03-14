#include "include/engine/vulkan_context.hpp"

#include <stdexcept>
#include <iostream>

VulkanContext::VulkanContext() :
    window()
{
    std::cout << "new vulkan context\n";
    init_vulkan();
    poll_main_loop();
}

VulkanContext::~VulkanContext() {
    vkDestroyInstance(instance, nullptr);
}

void VulkanContext::init_vulkan() {
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

    VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);

    if(res != VK_SUCCESS) {
        throw std::runtime_error("something went wrong");
    } else {
        std::cout << "vulkan content is ready\n";
    }
}

void VulkanContext::poll_main_loop() {
    while(window.is_alive()) {
        glfwPollEvents();
    }
}