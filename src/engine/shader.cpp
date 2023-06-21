#include "include/engine/shader.hpp"
#include "include/engine/vulkan_context.hpp"

#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace engine {
    Shader::Shader(std::string filepath, VulkanContext *context, VkShaderStageFlagBits shader_type) : 
        vulkan_context(context),
        path(filepath), 
        type(shader_type),
        shader_module{},
        stage_create_info{}
    {
        shader_module = create_shader_module(read_shader_file(path));
        stage_create_info = shader_stage_creation_info(type, shader_module);
    }

    std::string Shader::read_shader_file(std::string filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if(!file.is_open())
            throw std::runtime_error("failed to open file " + filepath);

        size_t file_size = (size_t)file.tellg(); 
        std::string result;
        result.reserve(file_size);

        file.seekg(0); //make sure we're reading from the beginning
        file.read(&result[0], file_size); //read data from file

        file.close();
        return result;
    }

    VkShaderModule Shader::create_shader_module(const std::string &code) {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(&code[0]);

        VkShaderModule sh_mod;
        if(vkCreateShaderModule(vulkan_context->device.logical, &create_info, nullptr, &sh_mod) != VK_SUCCESS)
            throw std::runtime_error("something went wrong when loading shader");
        else
            return sh_mod;
        return VK_NULL_HANDLE;
    }
    
    inline VkPipelineShaderStageCreateInfo Shader::shader_stage_creation_info(VkShaderStageFlagBits flag, VkShaderModule &mod) {
        VkPipelineShaderStageCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        create_info.stage = flag;
        create_info.module = mod;
        create_info.pName = "main"; //because why tf would you want to have a starting point not at main?
        return create_info;
    }
}

/*void Shader::load_shader_family(std::string filepath) { 
        VkShaderModule vertex_shader_module = create_shader_module(read_shader_file(filepath + "/vert.spv"));
        VkShaderModule fragment_shader_module = create_shader_module(read_shader_file(filepath + "/frag.cpv"));

        VkPipelineShaderStageCreateInfo vert_shader_create_info = shader_stage_creation_info(VK_SHADER_STAGE_VERTEX_BIT, vertex_shader_module);
        VkPipelineShaderStageCreateInfo frag_shader_create_info = shader_stage_creation_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader_module);
        VkPipelineShaderStageCreateInfo shader_stages_create_info[] = {vert_shader_create_info, frag_shader_create_info};

        VkPipelineVertexInputStateCreateInfo vertex_input_info{};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkPipelineVertexInputStateCreateInfo vertex_input_create_info{};

        //it's said to implictly destroy these
        vkDestroyShaderModule(vulkan_context->device.logical, vertex_shader_module, nullptr);
        vkDestroyShaderModule(vulkan_context->device.logical, fragment_shader_module, nullptr);
    }*/