#pragma once 

#include "include/vulkan/vertex.hpp"
#include "include/vulkan/vulkan_utils.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    class Mesh {
        VulkanContext *context;
    public:
        Mesh() = delete;
    };
}