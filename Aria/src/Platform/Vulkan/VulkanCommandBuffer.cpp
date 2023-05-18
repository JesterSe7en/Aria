#include "VulkanCommandBuffer.h"

#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "glm/fwd.hpp"
#include "vulkan/vk_enum_string_helper.h"

#include <cstdint>

namespace ARIA {

VulkanCommandBuffer::VulkanCommandBuffer() {
  VkCommandBufferAllocateInfo buffer_alloc_info;
  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_alloc_info.pNext = nullptr;
  buffer_alloc_info.commandPool = VulkanRendererAPI::get_vk_command_pool();
  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_alloc_info.commandBufferCount = 1;

  VkResult result = vkAllocateCommandBuffers(VulkanRendererAPI::get_vk_device(), &buffer_alloc_info, &mCommandBuffer);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result))
  }
}

bool VulkanCommandBuffer::begin_recording() {
  VkCommandBufferBeginInfo cmd_buffer_begin;
  cmd_buffer_begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmd_buffer_begin.flags = 0;
  cmd_buffer_begin.pNext = nullptr;
  cmd_buffer_begin.pInheritanceInfo = nullptr;

  VkResult result = vkBeginCommandBuffer(mCommandBuffer, &cmd_buffer_begin);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to begin recording command buffer - {0}", string_VkResult(result))
  }

  return result;
}

void VulkanCommandBuffer::draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex,
                               std::uint32_t firstInstance) {
  vkCmdDraw(mCommandBuffer, 3, 1, 0, 0);
}

void VulkanCommandBuffer::start_render_pass(glm::vec4 color) {
  // TODO: can we check if begin recording is called first? assert
  VkRenderPassBeginInfo render_pass_begin;
  render_pass_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin.pNext = nullptr;
  render_pass_begin.renderPass = mRenderPass;
  render_pass_begin.framebuffer = mSwapChainFrameBuffers[image_idx];
  render_pass_begin.renderArea.offset = {0, 0};
  render_pass_begin.renderArea.extent = mSwapChainExtent;

  VkClearValue clear_color = {{{color.r, color.g, color.b, color.a}}};  // black
  render_pass_begin.clearValueCount = 1;
  render_pass_begin.pClearValues = &clear_color;

  vkCmdBeginRenderPass(mCommandBuffer, &render_pass_begin, VK_SUBPASS_CONTENTS_INLINE);
}

bool VulkanCommandBuffer::reset() {
  VkResult result = vkResetCommandBuffer(mCommandBuffer, 0);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to begin recording command buffer - {0}", string_VkResult(result))
  }
  return result;
}

bool VulkanCommandBuffer::bind() {
  vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanRendererAPI::get_vk_graphics_pipeline());
}

bool VulkanCommandBuffer::end_recording() {
  VkResult result = vkEndCommandBuffer(mCommandBuffer);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to finish recording command buffer - {0}", string_VkResult(result))
  }
  return result;
}

}  // namespace ARIA