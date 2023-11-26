#include "include/engine/frame.hpp"
#include "include/engine/vulkan_context.hpp"

#include <iostream>

namespace engine {
    Frame::Frame(VulkanContext *vulkan_context) : 
        context(vulkan_context), 
        command_pool(context, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT),
        command_buffer(context, command_pool.command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY),
        fence(vulkan::create_fence(context->device.logical)),
        present_semaphore(vulkan::create_semaphore(context->device.logical)),
        graphics_semaphore(vulkan::create_semaphore(context->device.logical))
    {
        std::cout << "frame created\n";
    }

    Frame::Frame(const Frame &fr) : 
        fence{fr.fence},
        present_semaphore{fr.present_semaphore},
        graphics_semaphore{fr.graphics_semaphore},
        command_buffer(fr.command_buffer),
        command_pool(fr.command_pool),
        context(fr.context)
    {
        
    }

    Frame::Frame(Frame &&fr) : 
        fence{fr.fence},
        present_semaphore{fr.present_semaphore},
        graphics_semaphore{fr.graphics_semaphore},
        command_buffer(fr.command_buffer),
        command_pool(fr.command_pool),
        context(std::move(fr.context))
    {
        fr.fence = NULL;
        fr.present_semaphore = NULL;
        fr.graphics_semaphore = NULL;
    }

    Frame::~Frame() {
        vkDestroyFence(context->device.logical, fence, nullptr);
        vkDestroySemaphore(context->device.logical, graphics_semaphore, nullptr);
        vkDestroySemaphore(context->device.logical, present_semaphore, nullptr);    
    }

    void Frame::wait_for_fence() {
        if(vkWaitForFences(context->device.logical, 1, &fence, true, 1000000000) != VK_SUCCESS)
            throw std::runtime_error("waiting for fence took too long");
        if(vkResetFences(context->device.logical, 1, &fence) != VK_SUCCESS)
            throw std::runtime_error("couldn't reset fence");
    }
}