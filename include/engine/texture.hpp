#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"
#include <memory>

namespace engine {
    class VulkanContext; //forward decl

    class Texture {
        private:
            VulkanContext *context;

        public:
            Texture(VulkanContext *vulkan_context);
            Texture(VulkanContext *vulkan_context, VkImageCreateInfo &img_create_info, VkImageViewCreateInfo &img_view_create_info);
            ~Texture();

            void load_from_file(std::string filepath); 

        private:
            VkImage img;
            VmaAllocation allocation;

            VkImageView img_view;
            
            void create_image(VkImageCreateInfo &create_info);
            void create_image_view(VkImageViewCreateInfo &create_info);
    };
}