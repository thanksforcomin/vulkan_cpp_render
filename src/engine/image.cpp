#include "include/engine/image.hpp"

namespace engine {
    Image::Image(VulkanContext *vulkan_context, VkImageLayout image_layout) : 
        context(vulkan_context), 
        layout(image_layout),
        extent(vulkan::extent_2d_to_3d(context->swap_chain.swap_chain_extent)),
        image(vulkan::memory::allocate_image(
            context->allocator,
            vulkan::extent_2d_to_3d(context->swap_chain.swap_chain_extent),
            context->swap_chain.swap_chain_image_format,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY
            )
        ),
        image_view(vulkan::create_image_view(
            vulkan_context->device.logical,
            image.image,
            context->swap_chain.swap_chain_image_format
            )
        )
    { }

    Image::Image(VulkanContext *vulkan_context, VkImageLayout image_layout, VkExtent3D img_extent, VkFormat format) :
        context(vulkan_context),
        layout(image_layout),
        extent(img_extent),
        image(vulkan::memory::allocate_image(
                context->allocator, 
                img_extent, 
                format, 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
                VMA_MEMORY_USAGE_GPU_ONLY
            )
        ),
        image_view(vulkan::create_image_view(
            vulkan_context->device.logical,
            image.image,
            format
            )
        )
    { }

    void Image::create_image(vulkan::allocated_buffer&& buffer, UploadContext& upload_context) {
        upload_context.immediate_submit([&](VkCommandBuffer &cmd) 
            {
                VkImageMemoryBarrier memory_barrier{vulkan::image_memory_barrier(
                    image.image,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                    )
                };
                vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &memory_barrier);

                VkBufferImageCopy copy_region{vulkan::get_buffer_image_copy_region(extent)};
                vkCmdCopyBufferToImage(cmd, buffer.buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);

                memory_barrier = vulkan::image_memory_barrier(
                    image.image,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    );
                vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &memory_barrier);
            }
        );
        vulkan::memory::deallocate_buffer(std::move(buffer));
    }
}