#include "include/engine/window.hpp"

#include <stdexcept>
#include <iostream>


GLFWwindow *engine::Window::window_ptr = nullptr;

namespace engine {
    Window::Window(const VkInstance &inst, unsigned int width, unsigned int height) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window_ptr = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
        std::cout << "window created\n";
    }

    Window::~Window() {
        glfwDestroyWindow(window_ptr);

        glfwTerminate();
    }
}