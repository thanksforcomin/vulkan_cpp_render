#pragma once

#include "include/engine/window.hpp"


class VulkanContext {
    public:
        VulkanContext();
        ~VulkanContext();

    private:
        void init_vulkan();
        void poll_main_loop();
        void clean();

        Window window;
        VkInstance instance;
};