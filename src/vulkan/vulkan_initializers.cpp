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
    VkInstance create_instance(std::string applicaion_name, std::vector<const char*> extensions) {
        std::vector<const char *> required_extensions = require_extensions();
        required_extensions.insert(required_extensions.end(), extensions.begin(), extensions.end());
        std::vector<const char *> validation_layers{"VK_LAYER_KHRONOS_validation"};

        if(!validation_layers_support(validation_layers))
            throw std::runtime_error("validation layers are not supported");

        VkApplicationInfo appInfo{}; // general info
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicaion_name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "minivan";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        std::cout << "minivan" << " "
                << " 1, 0, 0 \n";

        VkInstanceCreateInfo createInfo = vulkan::instance_create_info(&appInfo, validation_layers, required_extensions);
        VkInstance inst;

        if(vkCreateInstance(&createInfo, nullptr, &inst) != VK_SUCCESS) 
            throw std::runtime_error("something went wrong");

        std::cout << "vulkan instance created\n";

        return inst;
    };

    /*
     * Creates a physical device for Vulkan rendering.
     */
    VkPhysicalDevice create_physical_device(VkInstance &inst, VkSurfaceKHR& surface, std::vector<const char*>& device_extensions) {
        unsigned int device_count = 0;
        vkEnumeratePhysicalDevices(inst, &device_count, nullptr);

        if (device_count == 0)
            std::runtime_error("no available devices, closing");

        std::vector<VkPhysicalDevice> available_devices(device_count);
        vkEnumeratePhysicalDevices(inst, &device_count, available_devices.data());

        for (auto &dev : available_devices)
        {
            if (is_device_valid(dev, surface, device_extensions))
            {
                VkPhysicalDeviceProperties device_properties;
                vkGetPhysicalDeviceProperties(dev, &device_properties);
                std::cout << device_properties.deviceName << " was chosen as the working device\n";
                return dev;
            }
        }

        throw std::runtime_error("no device is good enough.");
        return VK_NULL_HANDLE;
    };
        
    /**
    * Creates a logical device for Vulkan graphics rendering.
    */
    VkDevice create_logical_device(VkPhysicalDevice &dev, VkSurfaceKHR &surface, std::vector<const char*>& device_extensions) {
        queue_family_indicies queue_family = find_queue_family(dev, surface);
        std::set<uint32_t> unique_family_queues = {queue_family.graphics_family.value(), queue_family.present_family.value()};

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
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

        VkDeviceCreateInfo create_info = vulkan::logical_device_create_info(&device_fetures, queue_create_infos, device_extensions); // logical device create info
        VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_features = vulkan::get_dynamic_rendering_features(); //is's two integers so not that bad
        create_info.pNext = &dynamic_rendering_features;
        
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

    VkSwapchainKHR create_swap_chain(vulkan_device &vulkan_dev, VkSurfaceKHR &surface, queue_family_indicies indicies, swap_chain_support_details support_details, 
                                     GLFWwindow *window, VkSurfaceFormatKHR format, VkExtent2D extent, VkPresentModeKHR present_mode) {

        VkSwapchainCreateInfoKHR create_info{ 
            swap_chain_create_info(window,
                                   2, 
                                   support_details, 
                                   format,
                                   extent,
                                   present_mode,
                                   surface,
                                   {indicies.graphics_family.value(), indicies.present_family.value()}, 
                                   VK_NULL_HANDLE) 
        };

        VkSwapchainKHR swap_chain;
        vkCreateSwapchainKHR(vulkan_dev.logical, &create_info, nullptr, &swap_chain);
        return swap_chain;
    }

    VkImage create_image(VkDevice &dev, VkFormat format, VkImageUsageFlags usage, VkExtent3D extent) {
        VkImage img;
        VkImageCreateInfo create_info{image_create_info(format, usage, extent)};
        if (vkCreateImage(dev, &create_info, nullptr, &img) != VK_SUCCESS)
            throw std::runtime_error("failed to create image\n");
        return img;
    }

    VkImageView create_image_view(VkDevice &dev, VkImage &image, VkFormat &format)
    {
        VkImageView image_view;
        VkImageViewCreateInfo create_info(image_view_create_info(image, format));
        if (vkCreateImageView(dev, &create_info, nullptr, &image_view) != VK_SUCCESS)
            throw std::runtime_error("failed to create image view\n");
        return image_view;
    }

    VkFramebuffer create_framebuffer(VkDevice &dev, VkRenderPass &render_pass, VkImageView *image_attachment, VkExtent2D extent, uint32_t attachment_count) {
        VkFramebufferCreateInfo create_info{framebuffer_create_info(image_attachment, render_pass, extent.width, extent.height, attachment_count)};
        VkFramebuffer framebuffer;
        if (vkCreateFramebuffer(dev, &create_info, nullptr, &framebuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create framebuffer\n");
        return framebuffer;
    }

    VkCommandPool create_command_pool(VkDevice &dev, VkCommandPoolCreateFlags flags, uint32_t queue_family_index) {
        VkCommandPoolCreateInfo create_info{command_pool_create_info(queue_family_index, flags)};
        VkCommandPool command_pool;
        
        if(vkCreateCommandPool(dev, &create_info, nullptr, &command_pool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool\n");
        return command_pool;
    }

    VkCommandBuffer allocate_command_buffer(VkDevice &dev, VkCommandPool &command_pool, VkCommandBufferLevel level, uint32_t count) {
        VkCommandBufferAllocateInfo create_info{command_buffer_allocate_info(command_pool, level, count)};
        VkCommandBuffer command_buffer;

        if (vkAllocateCommandBuffers(dev, &create_info, &command_buffer) != VK_SUCCESS)
            throw std::runtime_error("cannot allocate main command buffer");

        return command_buffer;
    }

    VkDescriptorSetLayout create_descriptor_set_layout(VkDevice &dev, std::vector<VkDescriptorSetLayoutBinding> &bindings) {
        VkDescriptorSetLayoutCreateInfo create_info{descriptor_set_layout_create_info(bindings.data(), bindings.size())};
        VkDescriptorSetLayout layout;
        if (vkCreateDescriptorSetLayout(dev, &create_info, nullptr, &layout) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor set layout\n");
        return layout;
    }

    VkDescriptorSet allocate_descriptor_set(VkDevice &dev, VkDescriptorPool &pool, VkDescriptorSetLayout &layout) {
        VkDescriptorSetAllocateInfo create_info{descriptor_set_allocate_info(pool, &layout)};
        VkDescriptorSet descriptor_set;
        if (vkAllocateDescriptorSets(dev, &create_info, &descriptor_set) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate descriptor set\n");
        return descriptor_set;
    }

    VkDescriptorPool create_descriptor_pool(VkDevice &dev, std::vector<VkDescriptorPoolSize> pool_sizes, uint32_t max_sets) {
        VkDescriptorPoolCreateInfo create_info{descriptor_pool_create_info(&pool_sizes[0], pool_sizes.size(), max_sets)};
        VkDescriptorPool descriptor_pool;
        if (vkCreateDescriptorPool(dev, &create_info, nullptr, &descriptor_pool) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor pool\n");
        return descriptor_pool;
    }

    VkFence create_fence(VkDevice &dev, VkFenceCreateFlags flags) {
        VkFenceCreateInfo create_info{fence_create_info(flags)};
        VkFence fence;
        if (vkCreateFence(dev, &create_info, nullptr, &fence) != VK_SUCCESS)
            throw std::runtime_error("failed to create fence\n");
        return fence;
    }

    VkSemaphore create_semaphore(VkDevice &dev, VkSemaphoreCreateFlags flags) {
        VkSemaphoreCreateInfo create_info{semaphore_create_info(flags)};
        VkSemaphore semaphore;
        if (vkCreateSemaphore(dev, &create_info, nullptr, &semaphore) != VK_SUCCESS)
            throw std::runtime_error("failed to create semaphore\n");
        return semaphore;
    };

    VkRenderPass create_render_pass(VkDevice &dev, std::vector<VkAttachmentDescription> attachments, 
                                    std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies) 
    {
        VkRenderPassCreateInfo create_info{render_pass_create_info(attachments.data(), attachments.size(), 
                                                                   subpasses.data(), subpasses.size(), 
                                                                   dependencies.data(), dependencies.size())};
        VkRenderPass render_pass;
        if (vkCreateRenderPass(dev, &create_info, nullptr, &render_pass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass\n");
        return render_pass;
    }

    VkPipelineLayout create_pipeline_layout(VkDevice &dev, std::vector<VkDescriptorSetLayout> set_layouts, std::vector<VkPushConstantRange> push_constants) {
        VkPipelineLayoutCreateInfo create_info{pipeline_layout_create_info(set_layouts.data(), set_layouts.size(), push_constants.data(), push_constants.size())};
        VkPipelineLayout pipeline_layout;
        if(vkCreatePipelineLayout(dev, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) 
            throw std::runtime_error("failed to create pipeline layout\n");
        return pipeline_layout;
    }

    VkPipeline create_compute_pipeline(VkDevice &dev, VkPipelineLayout &pipeline_layout, VkPipelineShaderStageCreateInfo &compute_shader) {
        VkComputePipelineCreateInfo create_info{compute_pipeline_create_info(pipeline_layout, compute_shader)};
        VkPipeline pipeline;
        if (vkCreateComputePipelines(dev, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS)
            throw std::runtime_error("failed to create compute pipeline\n");
        return pipeline;
    }

    VkShaderModule create_chader_module(VkDevice &dev, std::vector<char>&& data) {
        VkShaderModuleCreateInfo create_info{shader_module_create_info(data.size(), &data[0])};
        VkShaderModule shader_module;
        if (vkCreateShaderModule(dev, &create_info, nullptr, &shader_module) != VK_SUCCESS)
            throw std::runtime_error("failed to create shader module\n");
        return shader_module;
    }
}