#include "include/engine/swap_chain.hpp"
#include "include/engine/vulkan_context.hpp"

#include <cstdint>
#include <limits>
#include <algorithm>
#include <iostream>
#include <stdexcept>


namespace engine {
    SwapChain::SwapChain(VulkanContext *context) : vulkan_context(context) {
        swap_chain_support_details details = vulkan_context->query_swap_chain_support();
        VkPresentModeKHR new_present_mode = choose_present_mode(details.present_modes);
        VkExtent2D new_extent = choose_extent(details.capabilities);
        VkSurfaceFormatKHR new_surface_format = choose_format(details.formats);

        int desired_image_count = details.capabilities.minImageCount + 1;
        if (details.capabilities.maxImageCount > 0 && desired_image_count > details.capabilities.maxImageCount) {
            desired_image_count = details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = vulkan_context->surface;
        create_info.minImageCount = desired_image_count;
        create_info.imageFormat = new_surface_format.format;
        create_info.imageColorSpace = new_surface_format.colorSpace;
        create_info.imageExtent = new_extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndicies indicies = vulkan_context->find_queue_family(vulkan_context->device.physical);
        uint32_t queue_family_indicies[] = {indicies.present_family.value(), indicies.graphics_family.value()};
        if(indicies.graphics_family != indicies.present_family) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indicies;
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0; //optional + no one cares
            create_info.pQueueFamilyIndices = nullptr;
        }

        create_info.preTransform = details.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //may rethink later
        create_info.presentMode = new_present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;

        VkResult res = vkCreateSwapchainKHR(vulkan_context->device.logical, &create_info, nullptr, &swap_chain);
        if (res != VK_SUCCESS) {
            throw std::runtime_error("swap chain creation failed\n");
        } else {
            std::cout << "swap chain created\n";
        }

        swap_chain_extent = new_extent;
        swap_chain_image_format = new_surface_format.format;

        query_swap_chain_images();
        create_image_views();
    }

    SwapChain::~SwapChain() {
        if(vulkan_context != nullptr && swap_chain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(vulkan_context->device.logical, swap_chain, nullptr);
            std::cout << vulkan_context << "\n";
        } else
            std::cout << vulkan_context << "\n";
    }

    VkPresentModeKHR SwapChain::choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes) {
        for (const auto& available_present_mode : available_present_modes) {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return available_present_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::choose_extent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;
        else {
            int width, height;
            glfwGetFramebufferSize(vulkan_context->window.window_ptr.get(), &width, &height);
            std::cout << "framebuffer size: " << width << " x " << height << "\n";

            VkExtent2D new_extent = {
                (uint32_t)width,
                (uint32_t)height
            };

            new_extent.width = std::clamp(new_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            new_extent.height = std::clamp(new_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return new_extent;
        }
    }

    VkSurfaceFormatKHR SwapChain::choose_format(const std::vector<VkSurfaceFormatKHR> &available_formats) {
        for (const auto& available_format : available_formats) {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return available_format;
            }
        }

        return available_formats[0];
    }

    void SwapChain::query_swap_chain_images() {
        uint32_t image_count;
        vkGetSwapchainImagesKHR(vulkan_context->device.logical, swap_chain, &image_count, nullptr);
        swap_chain_images.resize(image_count);
        vkGetSwapchainImagesKHR(vulkan_context->device.logical, swap_chain, &image_count, swap_chain_images.data());
    }

    void SwapChain::create_image_views() {
        swap_chain_image_views.resize(swap_chain_images.size());
        for (int i = 0; i < swap_chain_images.size(); i++) {
            VkImageViewCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = swap_chain_images[i];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = swap_chain_image_format;

            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.layerCount = 1;

            if (vkCreateImageView(vulkan_context->device.logical, &create_info, nullptr, &swap_chain_image_views[i]) != VK_SUCCESS) {
                throw std::runtime_error("cannot create image view");
            };
        }
    }

    void SwapChain::create_framebuffers(RenderPass &render_pass) {
        swap_chain_framebuffers.reserve(swap_chain_image_views.size());
        for (int i = 0; i < swap_chain_image_views.size(); i++)
        {
            VkFramebufferCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.pNext = nullptr;
            create_info.renderPass = render_pass.data;
            create_info.attachmentCount = 1;
            create_info.width = swap_chain_extent.width;
            create_info.height = swap_chain_extent.height;
            create_info.layers = 1;

            for (int i = 0; i < swap_chain_image_views.size(); i++) {
                create_info.pAttachments = &swap_chain_image_views[i];
                swap_chain_framebuffers.push_back(Framebuffer(vulkan_context, create_info));
            }
        }
    }

    Framebuffer& SwapChain::query_framebuffer(uint32_t index) {
        return swap_chain_framebuffers[index];
    }

    uint32_t SwapChain::query_next_image(VkSemaphore &semop) {
        uint32_t img_index;
        if(vkAcquireNextImageKHR(vulkan_context->device.logical, swap_chain, 1000000000, semop, nullptr, &img_index) != VK_SUCCESS)
            throw std::runtime_error("failed to aquire next image index");
        return img_index;
    }
}