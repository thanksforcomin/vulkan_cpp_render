#include "include/engine/swap_chain.hpp"
#include "include/engine/vulkan_context.hpp"
#include "include/vulkan/vulkan_utils.hpp"

#include <cstdint>
#include <limits>
#include <algorithm>
#include <iostream>
#include <stdexcept>


namespace engine {
    SwapChain::SwapChain(VulkanContext *context) : 
        vulkan_context(context)
    {   
        vulkan::queue_family_indicies indicies = vulkan::find_queue_family(context->device.physical, context->surface);
        vulkan::swap_chain_support_details support_details = vulkan::get_swap_chain_support(context->device.physical, context->surface);

        swap_chain_extent = vulkan::choose_extent(support_details.capabilities, context->game_window);
        VkSurfaceFormatKHR surface_format = vulkan::choose_format(support_details.formats);
        swap_chain_image_format = surface_format.format;

        swap_chain = vulkan::create_swap_chain(context->device, context->surface, indicies, support_details, 
                                               context->game_window, surface_format, swap_chain_extent, vulkan::choose_present_mode(support_details.present_modes));

        swap_chain_images = vulkan::get_swap_chain_images(context->device.logical, swap_chain);
        create_image_views();
    }

    SwapChain::~SwapChain() {
        if(vulkan_context != nullptr && swap_chain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(vulkan_context->device.logical, swap_chain, nullptr);
            std::cout << vulkan_context << "\n";
        } else
            std::cout << vulkan_context << "\n";
    }

    void SwapChain::create_image_views() {
        for (VkImage& image : swap_chain_images) {
            swap_chain_image_views.push_back(vulkan::create_image_view(vulkan_context->device.logical, image, swap_chain_image_format));
        }
    }

    void SwapChain::create_framebuffers(RenderPass &render_pass) {
        for (VkImageView& image_view : swap_chain_image_views) {
            swap_chain_framebuffers.push_back(
                vulkan::create_framebuffer(vulkan_context->device.logical, render_pass.render_pass, &image_view, swap_chain_extent)
            );
        }
    }

    VkFramebuffer& SwapChain::query_framebuffer(uint32_t index) {
        return swap_chain_framebuffers[index];
    }

    uint32_t SwapChain::query_next_image(VkSemaphore &semop) {
        uint32_t img_index;
        if(vkAcquireNextImageKHR(vulkan_context->device.logical, swap_chain, 1000000000, semop, nullptr, &img_index) != VK_SUCCESS)
            throw std::runtime_error("failed to aquire next image index");
        return img_index;
    }
}