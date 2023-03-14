#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/engine/defines.hpp"

class Window {
    private:
        GLFWwindow *window;
    public:
        Window(unsigned int width = WIDTH, unsigned int height = HEIGHT);
        ~Window();

        inline bool is_alive() { return !glfwWindowShouldClose(window); };
};