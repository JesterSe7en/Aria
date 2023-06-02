#include "VulkanLayer.h"

VulkanLayer::VulkanLayer() : Layer("Vulkan Layer") {
  vertex_shader_ =
      shader_library_.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/vert.spv", aria::ShaderType::VERTEX);
  frag_shader_ =
      shader_library_.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/frag.spv", aria::ShaderType::FRAGMENT);
}

VulkanLayer::~VulkanLayer() {
  //  auto vk_device = aria::VulkanDeviceManager::GetInstance().GetLogicalDevice();
  //
  //  vkDestroySemaphore(vk_device, image_available_semaphore_, nullptr);
  //  vkDestroySemaphore(vk_device, render_finished_semaphore_, nullptr);
  //  vkDestroyFence(vk_device, in_flight_fence_, nullptr);
}

void VulkanLayer::OnUpdate(aria::Timestep delta_time) {
  //  auto vk_device = aria::VulkanDeviceManager::GetInstance().GetLogicalDevice();

  //  vkWaitForFences(vk_device, 1, &in_flight_fence_, VK_TRUE, UINT64_MAX);
  //  vkResetFences(vk_device, 1, &in_flight_fence_);
  //
  //  std::uint32_t image_idx;
  //  VkResult result = vkAcquireNextImageKHR(vk_device, aria::VulkanSwapChain::GetInstance().GetSwapChain(), UINT64_MAX,
  //                                          image_available_semaphore_, in_flight_fence_, &image_idx);
  //  if (result != VK_SUCCESS) {
  //    ARIA_ERROR("Cannot get next image from swap chain - {0}", string_VkResult(result))
  //  }

  //  command_buffer_.Reset();
  //  command_buffer_.StartRecording();
  //  command_buffer_.StartRenderPass();
  //  command_buffer_.Bind();
  //
  //  command_buffer_.Draw(3, 1, 0, 0);
  //
  //  command_buffer_.EndRenderPass();
  //  command_buffer_.EndRecording();
}

void VulkanLayer::OnEvent(aria::Event &event) {}

void VulkanLayer::OnImGuiRender() {}
