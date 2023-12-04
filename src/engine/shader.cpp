#include "include/engine/shader.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/vulkan/pipeline.hpp"

#include "include/utils/utils.hpp"

#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace engine {
    Shader::Shader(VulkanContext *context, std::string filepath, VkShaderStageFlagBits shader_type) : 
        vulkan_context(context),
        path(filepath), 
        type(shader_type),
        shader_module(vulkan::create_chader_module(context->device.logical, utils::read_from_file(filepath))),
        stage_create_info{vulkan::pipeline::shader_state(type, shader_module)}
    { }

    Shader::~Shader() {
        vkDestroyShaderModule(vulkan_context->device.logical, shader_module, nullptr);
    }
}