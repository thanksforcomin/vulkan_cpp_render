#include "include/engine/texture.hpp"
#include "include/engine/vulkan_context.hpp" 

#define STB_IMAGE_IMPLEMENTATION

#include <cstring>

namespace engine {
    Texture::Texture(VulkanContext *vulkan_context) {
        //create_image(defaults::_get_image_create_info())
    }

    Texture::Texture(VulkanContext *vulkan_context, VkImageCreateInfo &img_create_info, VkImageViewCreateInfo &img_view_create_info) {

    }

    void Texture::load_from_file(std::string filepath) {
        /*int width;
        int height;
        int channels;

        stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if(!data) 
            throw std::runtime_error("failed to load image " + filepath);

        void* data_ptr = data;
        VkDeviceSize img_size = width * height * STBI_rgb_alpha;

        void* buf;
        vmaMapMemory(context->allocator, allocation, &buf);
        std::memcpy(buf, data_ptr, (size_t)img_size);
        vmaUnmapMemory(context->allocator, allocation);*/
    }

    
}