#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "include/engine/defines.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader_loader.hpp"
#include "include/engine/frame.hpp"
#include "include/engine/renderpass.hpp"
#include "include/engine/descriptor.hpp"
#include "include/engine/fwdplus.hpp"
#include "include/engine/dynamic_rendering.hpp"
#include "include/engine/image.hpp"
#include "include/engine/rendering_context.hpp"

#include "include/vulkan/pipeline.hpp"
#include "include/vulkan/vertex.hpp"
#include "include/vulkan/builder.hpp"

#include <iostream>
#include <memory>

struct example {
    int u;
};

int main() {
    //engine::compile_shader("../res/basic_shader/shader.vert");
    std::vector<const char*> instance_extensions{VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};
    std::vector<const char*> device_extensions{VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, 
                                               VK_KHR_MAINTENANCE2_EXTENSION_NAME,
                                               VK_KHR_MULTIVIEW_EXTENSION_NAME,
                                               VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
                                               VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
                                               VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                               VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME};

    engine::VulkanContext context(instance_extensions, device_extensions);

    uint32_t curr_frame = 0;

    //todo: clean up the mess
    std::vector<std::unique_ptr<engine::Frame>> frames;
    frames.push_back(std::move(std::unique_ptr<engine::Frame>(new engine::Frame(&context))));
    frames.push_back(std::move(std::unique_ptr<engine::Frame>(new engine::Frame(&context))));

    engine::RenderPass main_render_pass(&context);
    main_render_pass.init_default();
    context.swap_chain.create_framebuffers(main_render_pass);

    engine::DynamicRenderPass depth_prepass(&context);
    engine::DynamicRenderPass main_pass(&context);
    engine::DynamicRenderPass light_culling_pass(&context);

    engine::DescriptorPool pool(&context);
    pool.push(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100);
    pool.push(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100);
    pool.push(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3);
    pool.init();

    VkPushConstantRange push_constant{vulkan::get_push_constant_range(sizeof(fwd_plus::push_constant_object), VK_SHADER_STAGE_FRAGMENT_BIT)};

    engine::DescriptorSetLayout transform_set(&context);
    transform_set.push_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, VK_SHADER_STAGE_VERTEX_BIT); //instance data
    transform_set.create_layout();

    engine::DescriptorSetLayout camera_set(&context);
    camera_set.push_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT); //camera data
    camera_set.create_layout();

    engine::DescriptorSetLayout light_culling_set(&context);
    light_culling_set.push_layout_binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0, VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT); //light culling results
    light_culling_set.push_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT); //for point lights
    light_culling_set.create_layout();

    engine::DescriptorSetLayout intermediate_set(&context);
    intermediate_set.push_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT); //that depth attachment
    intermediate_set.create_layout();

    engine::DescriptorSetLayout material_set(&context);
    material_set.push_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, VK_SHADER_STAGE_FRAGMENT_BIT); //material properties
    material_set.push_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT); //albedo map
    material_set.push_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, VK_SHADER_STAGE_FRAGMENT_BIT); //normal map
    material_set.create_layout();

    engine::Shader vertex_shader(&context, "../res/light_culling/vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    engine::Shader fragment_shader(&context, "../res/light_culling/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    engine::Shader depth_shader(&context, "../res/light_culling/depth.spv", VK_SHADER_STAGE_VERTEX_BIT);

    VkPipelineLayout main_layout(
        vulkan::create_pipeline_layout(context.device.logical, 
            {
                transform_set.layout,
                camera_set.layout,
                light_culling_set.layout, 
                intermediate_set.layout, 
                material_set.layout
            }, 
            {
                push_constant
            }
        )
    );

    VkPipeline graphics_pipeline(
        fwd_plus::create_classic_pipeline(
            context, 
            main_layout, 
            vertex_shader, 
            fragment_shader
        )
    );
    VkPipeline depth_pipeline(
        fwd_plus::create_depth_pipeline(
            context, 
            main_layout, 
            depth_shader
        )
    );
    
    engine::RenderingContext depth_context();
    engine::RenderingContext rendering_context(VkRect2D {{0, 0}, context.swap_chain.swap_chain_extent});

    std::cout << "hello\n";
    
    //main loop
    while(!glfwWindowShouldClose(context.game_window)) {
        std::unique_ptr<engine::Frame> &frame = frames[(curr_frame++)%2];

        frame->wait_for_fence();

        uint32_t swap_chain_image_index = context.swap_chain.query_next_image(frame->present_semaphore);
        //std::cout << curr_frame << "\n"; //debug purposes
        frame->command_buffer.reset();

        main_render_pass.begin_info.framebuffer = context.swap_chain.query_framebuffer(swap_chain_image_index);

        frame->command_buffer.begin();
        frame->command_buffer.begin_renderpass(main_render_pass.begin_info);
        frame->command_buffer.end_renderpass();
        frame->command_buffer.end();

        frame->command_buffer.submit({frame->present_semaphore}, {frame->graphics_semaphore}, frame->fence, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        context.present(frame.get(), &swap_chain_image_index);

        glfwPollEvents();
    }
}