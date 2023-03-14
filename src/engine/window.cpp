#include "include/engine/window.hpp"

Window::Window(unsigned int width, unsigned int height) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

Window::~Window() {
    glfwDestroyWindow(window);

    glfwTerminate();
}
