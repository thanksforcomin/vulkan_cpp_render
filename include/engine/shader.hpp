#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

/*one shader class is a set of information and data about one shader to be put inside a pipeline*/

namespace engine {
    class VulkanContext; //forward decl

    class Shader {
        private:
            const VkShaderStageFlagBits type;
            const VulkanContext *vulkan_context;
        public:
            Shader(std::string filepath, VulkanContext *context, VkShaderStageFlagBits shader_type = VK_SHADER_STAGE_ALL);
            ~Shader();

            std::string path;
            VkShaderModule shader_module;
            VkPipelineShaderStageCreateInfo stage_create_info;

        private:
            std::string read_shader_file(std::string filepath);
            VkShaderModule create_shader_module(const std::string &code);
            inline VkPipelineShaderStageCreateInfo shader_stage_creation_info(VkShaderStageFlagBits flag, VkShaderModule &mod);
    };
}