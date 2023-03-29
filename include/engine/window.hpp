#pragma once

#include "include/engine/defines.hpp"
#include "include/engine/details.hpp"

namespace engine {
    class Window {
        public:
            static GLFWwindow *window_ptr;

        public:
            Window(const VkInstance &inst, unsigned int width = WIDTH, unsigned int height = HEIGHT);
            ~Window();

            inline bool is_alive() { return !glfwWindowShouldClose(window_ptr); };
    };
}