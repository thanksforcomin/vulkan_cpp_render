#include "include/vulkan/vulkan_initializers.hpp"
#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_create_infos.hpp"

#include <iostream>
#include <set>

namespace glfw {
    GLFWwindow* create_window(uint32_t height, uint32_t width) {
        if(!glfwInit()) throw std::runtime_error("funky!\n GLFW is not inited");

        if(!glfwVulkanSupported()) throw std::runtime_error("funky!\n vulkan is not supported");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        return glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    }
}

namespace vulkan {
    /*
     * Creates a Vulkan surface for a GLFW window.
     */
    VkSurfaceKHR create_surface(VkInstance &inst, GLFWwindow *window) {
        VkSurfaceKHR res;
        if (glfwCreateWindowSurface(inst, window, nullptr, &res) != VK_SUCCESS)
            throw std::runtime_error("failed to create window surface\n");
        return res;
    }

    /*
     * Creates a Vulkan instance with the specified application name.
     */
    VkInstance create_instance(std::string applicaion_name) {
        std::vector<const char *> required_extensions = require_extensions();
        std::vector<const char *> validation_layers{"VK_LAYER_KHRONOS_validation"};

        if(!validation_layers_support(validation_layers))
            throw std::runtime_error("validation layers are not supported");

        VkApplicationInfo appInfo{}; // general info
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicaion_name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "minivan";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        std::cout << "minivan" << " "
                << " 1, 0, 0 \n";

        VkInstanceCreateInfo createInfo = instance_create_info(&appInfo, validation_layers, required_extensions);
        VkInstance inst;

        if(vkCreateInstance(&createInfo, nullptr, &inst) != VK_SUCCESS) 
            throw std::runtime_error("something went wrong");

        std::cout << "vulkan instance created\n";

        return inst;
    };

    /*
     * Creates a physical device for Vulkan rendering.
     */
    VkPhysicalDevice create_physical_device(VkInstance &inst, VkSurfaceKHR& surface) {
        unsigned int device_count = 0;
        vkEnumeratePhysicalDevices(inst, &device_count, nullptr);

        if (device_count == 0)
            std::runtime_error("no available devices, closing");

        std::vector<VkPhysicalDevice> available_devices(device_count);
        vkEnumeratePhysicalDevices(inst, &device_count, available_devices.data());

        for (auto &dev : available_devices)
        {
            if (is_device_valid(dev, surface))
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
        
    /**
    * Creates a logical device for Vulkan graphics rendering.
    */
    VkDevice create_logical_device(VkPhysicalDevice &dev, VkSurfaceKHR &surface) {
        queue_family_indicies queue_family = find_queue_family(dev, surface);
        std::set<uint32_t> unique_family_queues = {queue_family.graphics_family.value(), queue_family.present_family.value()};

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::vector<const char *> required_device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        VkPhysicalDeviceFeatures device_fetures = {};

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

        VkDeviceCreateInfo create_info = logical_device_create_info(&device_fetures, queue_create_infos, required_device_extensions); // logical device create info

        // i have no idea what im doing please help
        VkDevice log_dev;
        if (vkCreateDevice(dev, &create_info, nullptr, &log_dev) != VK_SUCCESS)
            std::runtime_error("failed to create device\n");

        return log_dev;
    }

    VkQueue create_queue(VkDevice &dev, uint32_t queue_family_index) {
        VkQueue queue;
        vkGetDeviceQueue(dev, queue_family_index, 0, &queue);
        return queue;
    }

    VkSwapchainKHR create_swap_chain(vulkan_device &dev, VkSurfaceKHR &surface, GLFWwindow *window) {
        queue_family_indicies indicies = find_queue_family(dev.physical, surface);

        VkSwapchainCreateInfoKHR create_info{ 
            swap_chain_create_info(window,
                                   1, 
                                   get_swap_chain_support(dev.physical, surface), 
                                   surface, 
                                   {indicies.graphics_family.value(), indicies.present_family.value()}, 
                                   VK_NULL_HANDLE) 
        };

        VkSwapchainKHR swap_chain;
        vkCreateSwapchainKHR(dev.logical, &create_info, nullptr, &swap_chain);
        return swap_chain;
    }

    VkImageView create_image_view(VkDevice &dev, VkImage &image, VkFormat &format)
    {
        VkImageView image_view;
        VkImageViewCreateInfo create_info(image_view_create_info(image, format));
        if (vkCreateImageView(dev, &create_info, nullptr, &image_view) != VK_SUCCESS)
            throw std::runtime_error("failed to create image view\n");
        return image_view;
    }

    VkFramebuffer create_framebuffer(VkDevice &dev, VkRenderPass &render_pass, VkImageView *image_attachment, VkExtent2D extent) {
        VkFramebufferCreateInfo create_info{framebuffer_create_info(image_attachment, render_pass, extent.width, extent.height)};
        VkFramebuffer framebuffer;
        if (vkCreateFramebuffer(dev, &create_info, nullptr, &framebuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create framebuffer\n");
        return framebuffer;
    }
}