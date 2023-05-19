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
  mCommandBuffer.start_recording();
  mCommandBuffer.start_render_pass();
  mCommandBuffer.bind();

  mCommandBuffer.draw(3, 1, 0, 0);

  mCommandBuffer.end_render_pass();
  mCommandBuffer.end_recording();
}

void VulkanLayer::on_event(ARIA::Event& event) {}

void VulkanLayer::on_imgui_render() {}
