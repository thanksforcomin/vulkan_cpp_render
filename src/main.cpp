#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "include/engine/defines.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/engine/shader_loader.hpp"

#include <iostream>

int main() {
    engine::compile_shader("../res/shader.vert");
    engine::VulkanContext();
}