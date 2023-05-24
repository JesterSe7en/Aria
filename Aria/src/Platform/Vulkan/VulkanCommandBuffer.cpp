#include "VulkanSwapChain.hpp"
#include "ariapch.h"
#include "VulkanCommandBuffer.hpp"
#include "Platform/Vulkan/VulkanRendererApi.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "vulkan/vk_enum_string_helper.h"

namespace aria {

VulkanCommandBuffer::VulkanCommandBuffer() {
  VkCommandBufferAllocateInfo buffer_alloc_info;
  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_alloc_info.pNext = nullptr;
  buffer_alloc_info.commandPool = VulkanRendererApi::GetInstance().GetVkCommandPool();
  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_alloc_info.commandBufferCount = 1;

  VkResult result = vkAllocateCommandBuffers(VulkanDeviceManager::GetInstance().GetLogicalDevice(), &buffer_alloc_info,
                                             &command_buffer_);
  if (result != VK_SUCCESS) { ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result)) }

  SetViewport();
  SetScissor();
}

void VulkanCommandBuffer::SetViewport() {
  auto details = VulkanSwapChain::GetInstance().GetSwapChainDetails();

  VkViewport viewport;
  viewport.x = viewport.y = 0.0f;
  viewport.width = static_cast<float>(details.swap_chain_extend_2d.width);
  viewport.height = static_cast<float>(details.swap_chain_extend_2d.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer_, 0, 1, &viewport);
}

void VulkanCommandBuffer::SetScissor() {
  auto details = VulkanSwapChain::GetInstance().GetSwapChainDetails();

  VkRect2D scissor;
  scissor.offset = {0, 0};
  scissor.extent = details.swap_chain_extend_2d;
  vkCmdSetScissor(command_buffer_, 0, 1, &scissor);
}

bool VulkanCommandBuffer::StartRecording() {
  VkCommandBufferBeginInfo cmd_buffer_begin;
  cmd_buffer_begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmd_buffer_begin.flags = 0;
  cmd_buffer_begin.pNext = nullptr;
  cmd_buffer_begin.pInheritanceInfo = nullptr;

  VkResult result = vkBeginCommandBuffer(command_buffer_, &cmd_buffer_begin);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to begin recording command buffer - {0}", string_VkResult(result))
  }

  return result;
}

void VulkanCommandBuffer::Draw(std::uint32_t vertex_count, std::uint32_t instance_count, std::uint32_t first_vertex,
                               std::uint32_t first_instance) {
  vkCmdDraw(command_buffer_, 3, 1, 0, 0);
}

void VulkanCommandBuffer::StartRenderPass(glm::vec4 color) {
  // TODO: can we check if begin recording is called first? assert

  auto details = VulkanSwapChain::GetInstance().GetSwapChainDetails();
  auto frame_buffers = VulkanGraphicsPipeline::GetInstance().GetFrameBuffers();

  VkRenderPassBeginInfo render_pass_begin;
  render_pass_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin.pNext = nullptr;
  render_pass_begin.renderPass = VulkanRendererApi::GetInstance().GetRenderPass();
  //  render_pass_begin.framebuffer = frame_buffers[image_idx];

  //FIXME: THis should be passing index
  render_pass_begin.framebuffer = frame_buffers[0];
  render_pass_begin.renderArea.offset = {0, 0};
  render_pass_begin.renderArea.extent = details.swap_chain_extend_2d;

  VkClearValue clear_color = {{{color.r, color.g, color.b, color.a}}};// black
  render_pass_begin.clearValueCount = 1;
  render_pass_begin.pClearValues = &clear_color;

  vkCmdBeginRenderPass(command_buffer_, &render_pass_begin, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::EndRenderPass() { vkCmdEndRenderPass(command_buffer_); }

bool VulkanCommandBuffer::Reset() {
  VkResult result = vkResetCommandBuffer(command_buffer_, 0);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to begin recording command buffer - {0}", string_VkResult(result))
  }
  return result;
}

bool VulkanCommandBuffer::Bind() {
  vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    VulkanGraphicsPipeline::GetInstance().GetGraphicsPipeline());
  return true;
}

bool VulkanCommandBuffer::EndRecording() {
  VkResult result = vkEndCommandBuffer(command_buffer_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to finish recording command buffer - {0}", string_VkResult(result))
  }
  return result;
}

}// namespace aria
