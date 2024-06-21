#pragma once

#include <optional>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine {
  class RenderingContext {
    private:
      VkRenderingInfo rendering_info;

      std::optional<VkRenderingAttachmentInfoKHR> color_attachment;
      std::optional<VkRenderingAttachmentInfoKHR> depth_attachment;
      std::optional<VkRenderingAttachmentInfoKHR> stencil_attachment;

    public:
      RenderingContext(VkRect2D rendering_area, uint32_t layer_count = 1, uint32_t color_attachment_count = 0);
      RenderingContext(VkRenderingInfo &&info);
      ~RenderingContext();

      void set_color_attachment(VkRenderingAttachmentInfoKHR &&attachment);
      void set_depth_attachment(VkRenderingAttachmentInfoKHR &&attachment);
      void set_stencil_attachment(VkRenderingAttachmentInfoKHR &&attachment);

      inline void set_color_image(VkImageView &img_view) {
        color_attachment->imageView = img_view;
      }
      inline void set_depth_image(VkImageView &img_view) {
        depth_attachment->imageView = img_view;
      }
      inline void set_stencil_image(VkImageView &img_view) {
        stencil_attachment->imageView = img_view;
      }

      void begin(VkCommandBuffer& cmd_buffer);
      void end(VkCommandBuffer& cmd_buffer);
  };
}