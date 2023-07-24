#include "include/engine/memory_context.hpp"
#include "include/engine/vulkan_context.hpp"

namespace engine {
    UploadContext::UploadContext(VulkanContext *vulkan_context) : 
        context(vulkan_context),
        command_dispatcher()
    {

    }
}