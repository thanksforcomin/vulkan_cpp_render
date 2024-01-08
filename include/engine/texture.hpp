#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"
#include <memory>

namespace engine {
    class VulkanContext; //forward decl

    class Image {
        private:
            VulkanContext *context;

        public:
            Image() = delete;
            Image(VulkanContext *vulkan_context);
            Image(VulkanContext *vulkan_context, VkImageCreateInfo &img_create_info, VkImageViewCreateInfo &img_view_create_info);
            ~Image();

            void load_from_file(std::string filepath); 

        private:
            VkImage img;
            VmaAllocation allocation;

            VkImageView img_view;
            
            void create_image(VkImageCreateInfo &create_info);
            void create_image_view(VkImageViewCreateInfo &create_info);
    };
}