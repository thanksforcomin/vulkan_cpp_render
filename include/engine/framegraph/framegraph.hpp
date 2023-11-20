#pragma once

#include "include/vulkan/vulkan_initializers.hpp"
#include "include/vulkan/vulkan_utils.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/buffers.hpp"
#include "include/engine/renderpass.hpp"

namespace engine {
    class FrameGraph {
        VulkanContext *context;

    public:
        FrameGraph(VulkanContext *vulkan_context);
        ~FrameGraph();

        Buffer& create_buffer();
        Image& create_image();
        DynamicRenderPass& create_render_pass();

        void bake();
    };
}