#include "include/engine/window.hpp"

#include <stdexcept>
#include <iostream>

namespace engine {
    Window::Window(unsigned int width, unsigned int height) :
        window_ptr(create_window(width, height), glfwDestroyWindow)
    {
        std::cout << window_ptr.get() << "\n";
        std::cout << "window created\n";
    }

    Window::~Window() {

        glfwTerminate();
    }

    GLFWwindow* Window::create_window(uint32_t width, uint32_t height) {
        if(!glfwInit()) throw std::runtime_error("funky!\n GLFW is not inited");

        if(!glfwVulkanSupported()) throw std::runtime_error("funky!\n vulkan is not supported");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        return glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    }
}