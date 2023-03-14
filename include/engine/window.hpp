#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/engine/defines.hpp"

class Window {
    public:
        Window(unsigned int width = WIDTH, unsigned int height = HEIGHT);
        ~Window();
    private:
        GLFWwindow *window;
};