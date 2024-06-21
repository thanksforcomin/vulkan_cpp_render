#include "include/engine/rendering_context.hpp"
#include "include/vulkan/vulkan_utils.hpp"

namespace engine {

  RenderingContext::~RenderingContext() {}

  RenderingContext::RenderingContext(VkRect2D rendering_area, uint32_t layer_count, uint32_t color_attachment_count) : 
    rendering_info(vulkan::get_rendering_info(rendering_area, nullptr, nullptr, nullptr, layer_count, color_attachment_count))
  {}

  void RenderingContext::set_color_attachment(VkRenderingAttachmentInfoKHR &&attachment) {
    rendering_info.pColorAttachments = &color_attachment.value();
    rendering_info.colorAttachmentCount++;
    color_attachment = std::move(attachment);
  }

  void RenderingContext::set_depth_attachment(VkRenderingAttachmentInfoKHR &&attachment) {
    rendering_info.pDepthAttachment = &depth_attachment.value();
    rendering_info.colorAttachmentCount++;
    depth_attachment = std::move(attachment);
  }

  void RenderingContext::set_stencil_attachment(VkRenderingAttachmentInfoKHR &&attachment) {
    rendering_info.pStencilAttachment = &stencil_attachment.value();
    rendering_info.colorAttachmentCount++;
    stencil_attachment = std::move(attachment);
  }

  void RenderingContext::begin(VkCommandBuffer& cmd_buffer) {
    vkCmdBeginRenderingKHR(cmd_buffer, &rendering_info);
  }

  void RenderingContext::end(VkCommandBuffer& cmd_buffer) {
    vkCmdEndRenderingKHR(cmd_buffer);
  }
}