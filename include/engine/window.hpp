#pragma once

#include "include/engine/defines.hpp"
#include "include/engine/details.hpp"

#include <memory>

namespace engine {
    class Window {
        public:
            std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window_ptr;

        public:
            Window(unsigned int width = WIDTH, unsigned int height = HEIGHT);
            ~Window();

            inline bool is_alive() { return !glfwWindowShouldClose(window_ptr.get()); };

        private:
            GLFWwindow* create_window(uint32_t width, uint32_t height);
    };
}