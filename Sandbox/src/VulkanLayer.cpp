#include "VulkanLayer.h"

#include "Aria/Core/Log.h"
#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/Shader.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "vulkan/vk_enum_string_helper.h"

#include <stdint.h>

#include <cstdint>

static VkDevice sDevice = ARIA::VulkanRendererAPI::get_vk_device();

VulkanLayer::VulkanLayer() : Layer("Vulkan Layer") {
  VkSemaphoreCreateInfo semaphore_info;
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphore_info.flags = 0;
  semaphore_info.pNext = nullptr;

  VkFenceCreateInfo fence_info;
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.flags =
      VK_FENCE_CREATE_SIGNALED_BIT;  // to avoid hangup on first frame - we are waiting on fences in on_update()
  fence_info.pNext = nullptr;

  bool result = vkCreateSemaphore(sDevice, &semaphore_info, nullptr, &mImageAvailableSemaphore) &&
                vkCreateSemaphore(sDevice, &semaphore_info, nullptr, &mRenderFinishedSemaphore) &&
                vkCreateFence(sDevice, &fence_info, nullptr, &mInFlightFence);
  if (result != VK_SUCCESS) {
    ARIA_ERROR("Cannot create semaphores - {0}", string_VkResult(static_cast<VkResult>(result)))
  }

  mVertexShader =
      mShaderLibrary.load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/vert.spv", ARIA::ShaderType::VERTEX);
  mFragShader =
      mShaderLibrary.load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/frag.spv", ARIA::ShaderType::FRAGMENT);
  ARIA::Renderer::create_pipeline();
}

VulkanLayer::~VulkanLayer() {
  vkDestroySemaphore(sDevice, mImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(sDevice, mRenderFinishedSemaphore, nullptr);
  vkDestroyFence(sDevice, mInFlightFence, nullptr);
}

void VulkanLayer::on_update(ARIA::Timestep delta_time) {
  vkWaitForFences(sDevice, 1, &mInFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(sDevice, 1, &mInFlightFence);

  std::uint32_t image_idx;
  VkResult result = vkAcquireNextImageKHR(sDevice, ARIA::VulkanRendererAPI::get_vk_swap_chain(), UINT64_MAX,
                                          mImageAvailableSemaphore, mInFlightFence, &image_idx);
  if (result != VK_SUCCESS) {
    ARIA_ERROR("Cannot get next image from swap chain - {0}", string_VkResult(result))
  }

  mCommandBuffer.reset();
  mCommandBuffer.begin_recording();

  VkRenderPassBeginInfo render_pass_begin;
  render_pass_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin.pNext = nullptr;
  render_pass_begin.renderPass = mRenderPass;
  render_pass_begin.framebuffer = mSwapChainFrameBuffers[image_idx];
  render_pass_begin.renderArea.offset = {0, 0};
  render_pass_begin.renderArea.extent = mSwapChainExtent;

  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};  // black
  render_pass_begin.clearValueCount = 1;
  render_pass_begin.pClearValues = &clear_color;

  vkCmdBeginRenderPass(mCommandBuffer, &render_pass_begin, VK_SUBPASS_CONTENTS_INLINE);

  mCommandBuffer.bind();

  VkViewport viewport;
  viewport.x = viewport.y = 0.0f;
  viewport.width = static_cast<float>(mSwapChainExtent.width);
  viewport.height = static_cast<float>(mSwapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);

  VkRect2D scissor;
  scissor.offset = {0, 0};
  scissor.extent = mSwapChainExtent;
  vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);

  mCommandBuffer.draw(3, 1, 0, 0);

  vkCmdEndRenderPass(mCommandBuffer);

  mCommandBuffer.end_recording();
}

void VulkanLayer::on_event(ARIA::Event& event) {}

void VulkanLayer::on_imgui_render() {}
