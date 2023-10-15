//
// builders are simple structs that can be used to create vulkan objects
// that require a lot of temporary data
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"

#include "include/vulkan/vulkan_utils.hpp"

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
}