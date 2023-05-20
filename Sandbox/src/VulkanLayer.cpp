#include "VulkanLayer.h"

#include "Aria/Core/Log.h"
#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/Shader.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#include "vulkan/vk_enum_string_helper.h"

#include <stdint.h>

#include <cstdint>

static VkDevice sDevice = aria::VulkanRendererApi::GetVkDevice();

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
      mShaderLibrary.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/vert.spv", aria::ShaderType::VERTEX);
  mFragShader =
      mShaderLibrary.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/frag.spv", aria::ShaderType::FRAGMENT);
  aria::Renderer::CreatePipeline();
}

VulkanLayer::~VulkanLayer() {
  vkDestroySemaphore(sDevice, mImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(sDevice, mRenderFinishedSemaphore, nullptr);
  vkDestroyFence(sDevice, mInFlightFence, nullptr);
}

void VulkanLayer::OnUpdate(aria::Timestep delta_time) {
  vkWaitForFences(sDevice, 1, &mInFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(sDevice, 1, &mInFlightFence);

  std::uint32_t image_idx;
  VkResult result = vkAcquireNextImageKHR(sDevice, aria::VulkanRendererApi::GetVkSwapChain(), UINT64_MAX,
                                          mImageAvailableSemaphore, mInFlightFence, &image_idx);
  if (result != VK_SUCCESS) {
    ARIA_ERROR("Cannot get next image from swap chain - {0}", string_VkResult(result))
  }

  mCommandBuffer.Reset();
  mCommandBuffer.StartRecording();
  mCommandBuffer.StartRenderPass();
  mCommandBuffer.Bind();

  mCommandBuffer.Draw(3, 1, 0, 0);

  mCommandBuffer.EndRenderPass();
  mCommandBuffer.EndRecording();
}

void VulkanLayer::OnEvent(aria::Event &event) {}

void VulkanLayer::OnImGuiRender() {}
