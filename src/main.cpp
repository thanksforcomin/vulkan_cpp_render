#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "include/engine/defines.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader_loader.hpp"
#include "include/engine/pipeline.hpp"
#include "include/engine/renderpass.hpp"

#include <iostream>

int main() {
    engine::compile_shader("../res/basic_shader/shader.vert");
    engine::VulkanContext context;

    VkAttachmentDescription color_attachment = engine::default_attachment(context.swap_chain.swap_chain_image_format);

    std::vector<engine::Frame> frames(2, &context);

    VkClearValue clear_val{};
    clear_val.color = {0.0f, 0.0f, 0.0f, 1.0f};

    engine::RenderPass main_render_pass(&context, 1, &clear_val);
    main_render_pass.init_default();
    context.swap_chain.create_framebuffers(main_render_pass);

    context.poll_main_loop();
}