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

#include "include/vulkan/pipeline.hpp"
#include "include/vulkan/vertex.hpp"

#include <iostream>
#include <memory>

struct example {
    int u;
};

int main() {
    //engine::compile_shader("../res/basic_shader/shader.vert");
    engine::VulkanContext context;

    uint32_t curr_frame = 0;

    //todo: clean up the mess
    std::vector<std::unique_ptr<engine::Frame>> frames;
    frames.push_back(std::move(std::unique_ptr<engine::Frame>(new engine::Frame(&context))));
    frames.push_back(std::move(std::unique_ptr<engine::Frame>(new engine::Frame(&context))));

    VkClearValue clear_val{};
    clear_val.color = {0.0f, 0.0f, 1.0f, 1.0f};

    engine::RenderPass main_render_pass(&context);
    main_render_pass.init_default();
    context.swap_chain.create_framebuffers(main_render_pass);

    engine::DescriptorPool pool(&context);
    pool.push(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100);
    pool.push(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100);
    pool.push(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3);
    pool.init();

    engine::DescriptorSet camera_descriptor(&context);

    //main loop
    while(!glfwWindowShouldClose(context.game_window)) {
        std::unique_ptr<engine::Frame> &frame = frames[(curr_frame++)%2];

        frame->wait_for_fence();

        uint32_t swap_chain_image_index = context.swap_chain.query_next_image(frame->present_semaphore);
        //std::cout << curr_frame << "\n"; //debug purposes
        frame->command_buffer.reset();

        main_render_pass.begin_info.framebuffer = context.swap_chain.query_framebuffer(swap_chain_image_index);

        frame->command_buffer.begin(main_render_pass.begin_info);
        frame->command_buffer.end();

        context.submit(frame.get());
        context.present(frame.get(), &swap_chain_image_index);

        glfwPollEvents();
    }
}