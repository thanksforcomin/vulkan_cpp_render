#pragma once

#include "include/engine/window.hpp"


class VulkanContext {
    public:
        VulkanContext() = default;
        ~VulkanContext() = default;

    private:
        void init_vulkan();
        void pollMainLoop();
        void clean();

        Window window;
        VkInstance instance;
};