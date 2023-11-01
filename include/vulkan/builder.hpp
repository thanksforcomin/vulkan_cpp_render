//
// builders are simple structs that can be used to create vulkan objects
// that require a lot of temporary data
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"

#include "include/vulkan/vulkan_utils.hpp"
#include "include/vulkan/vulkan_initializers.hpp"

#include <optional>
#include <functional>
#include <vector>

namespace vulkan {
    struct descriptor_set_update_builder {
        std::vector<VkDescriptorBufferInfo> descriptor_buffers;
        std::vector<VkWriteDescriptorSet> write_descriptor_sets;

        descriptor_set_update_builder push_buffer_binding(vulkan::allocated_buffer& buffer, VkDescriptorType type, VkDescriptorSet &descriptor_set, uint32_t binding) {
            descriptor_buffers.push_back(vulkan::get_buffer_info(buffer.buffer, buffer.size));
            write_descriptor_sets.push_back(vulkan::get_descriptor_write_info(type, descriptor_set, binding, descriptor_buffers.back()));
            return *this;
        }

        void update(VkDevice &dev) {
            vkUpdateDescriptorSets(dev, (uint32_t)write_descriptor_sets.size(), &write_descriptor_sets[0], 0, nullptr);
        }
    };

    struct renderpass_builder {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkAttachmentReference> attachment_references;
        std::vector<VkSubpassDescription> subpasses;
        std::vector<VkSubpassDependency> dependencies;

        renderpass_builder push_subpass(VkPipelineBindPoint bind_point) {
            subpasses.push_back(VkSubpassDescription {
                    .pipelineBindPoint = bind_point
                }
            );
            return *this;
        }

        renderpass_builder push_color_attachment(VkFormat format, VkImageLayout fin_layout, VkImageLayout init_layout) {
            attachments.push_back(vulkan::get_color_attachment(format, fin_layout, init_layout));
            attachment_references.push_back(VkAttachmentReference {(uint32_t)attachments.size() - 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
            subpasses.back().colorAttachmentCount += 1;
            subpasses.back().pColorAttachments = &attachment_references.back();
            return *this;
        }

        renderpass_builder push_depth_attachment(VkFormat format, VkImageLayout fin_layout, VkImageLayout init_layout) {
            attachments.push_back(vulkan::get_depth_attachment(format, fin_layout, init_layout));
            attachment_references.push_back(VkAttachmentReference {(uint32_t)attachments.size() - 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
            subpasses.back().pDepthStencilAttachment = &attachment_references.back();
            return *this;
        }

        //TODO: make it make sense
        renderpass_builder push_resolve_attachment(VkFormat format, VkImageLayout fin_layout, VkImageLayout init_layout) {
            attachments.push_back(vulkan::get_color_attachment(format, fin_layout, init_layout));
            attachment_references.push_back(VkAttachmentReference {(uint32_t)attachments.size() - 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
            subpasses.back().pResolveAttachments = &attachment_references.back();
            return *this;
        }

        renderpass_builder push_dependency(uint32_t dst_subpass = 0) {
            dependencies.push_back(vulkan::get_subpass_dependency(dst_subpass));
            return *this;
        }

        VkRenderPass create(VkDevice &dev) {
            return vulkan::create_render_pass(dev, attachments, subpasses, dependencies);
        }
    };
}