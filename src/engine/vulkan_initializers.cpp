#include "include/engine/vulkan_initializers.hpp"
#include "include/engine/vulkan_utils.hpp"

#include <iostream>
#include <set>

namespace engine {
    namespace vulkan {
        std::vector<const char*> get_required_extensions() {
            uint32_t glfwExtensionCount = 0;
            const char **glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            std::cout << "Extensions: " << glfwExtensions << "\n";

            return extensions;
        };

        VkSurfaceKHR create_surface(VkInstance &inst, GLFWwindow *window) {
            VkSurfaceKHR res;
            if (glfwCreateWindowSurface(inst, window, nullptr, &res) != VK_SUCCESS)
                throw std::runtime_error("failed to create window surface\n");
            return res;
        }

        VkInstance create_instance(std::string applicaion_name) {
            std::vector<const char *> required_extensions = get_required_extensions();
            /*
            if (IS_DEBUG && !check_validation_layers_support())
            {
                throw std::runtime_error("validation layers are not available");
            }
            */
            VkApplicationInfo appInfo{}; // general info
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = applicaion_name.c_str();
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "minivan";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            std::cout << "minivan" << " "
                    << " 1, 0, 0 \n";

            VkInstanceCreateInfo createInfo{}; // like a config to a ulkan context
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = (uint32_t)required_extensions.size();
            createInfo.ppEnabledExtensionNames = required_extensions.data();

            std::vector<const char *> validation_layers{"VK_LAYER_KHRONOS_validation"};

            createInfo.enabledLayerCount = (uint32_t)validation_layers.size();
            createInfo.ppEnabledLayerNames = &validation_layers[0];

            VkInstance inst;

            if(vkCreateInstance(&createInfo, nullptr, &inst) != VK_SUCCESS) 
                throw std::runtime_error("something went wrong");

            std::cout << "vulkan instance created\n";

            return inst;
        };
    
        VkPhysicalDevice create_physical_device(VkInstance &inst) {
            unsigned int device_count = 0;
            vkEnumeratePhysicalDevices(inst, &device_count, nullptr);

            if (device_count == 0)
                std::runtime_error("no available devices, closing");

            std::vector<VkPhysicalDevice> available_devices(device_count);
            vkEnumeratePhysicalDevices(inst, &device_count, available_devices.data());

            for (auto &dev : available_devices)
            {
                if (is_device_valid(dev))
                {
                    VkPhysicalDeviceProperties device_properties;
                    vkGetPhysicalDeviceProperties(dev, &device_properties);
                    std::cout << device_properties.deviceName << " was chosen as the working device\n";
                    return dev;
                }
            }

            std::runtime_error("no device is good enough.");
            return VK_NULL_HANDLE;
        };
    
        VkDevice create_logical_device(VkPhysicalDevice &dev) {
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
    }
}