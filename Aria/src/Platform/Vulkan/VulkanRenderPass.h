#pragma once

#include "VulkanSwapChain.h"
#include "Aria/Core/Base.h"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanRenderPass {
 public:
  ~VulkanRenderPass();

  VulkanRenderPass(const VulkanRenderPass &) = delete;
  VulkanRenderPass(VulkanRenderPass &&) = delete;
  VulkanRenderPass &operator=(const VulkanRenderPass &) = delete;
  VulkanRenderPass &operator=(VulkanRenderPass &&) = delete;

  static Ref<VulkanRenderPass> Create();
  VkRenderPass GetRenderPass() { return vk_render_pass_; }

 private:
  VkDevice vk_device_ = VK_NULL_HANDLE;
  VulkanSwapChain vulkan_swap_chain_;
  VkRenderPass vk_render_pass_ = VK_NULL_HANDLE;

  VulkanRenderPass();
};

}// namespace aria
