#pragma once

#include "Aria/Core/Base.h"
#include "VulkanLib.h"
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
  VkRenderPass GetVkRenderPass() { return vk_render_pass_; }

 private:
  VulkanLib vklib_;
  VkRenderPass vk_render_pass_ = VK_NULL_HANDLE;

  VulkanRenderPass();
};

}// namespace aria
