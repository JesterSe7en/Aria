#pragma once

#include "Aria/Core/Base.h"
#include "VulkanDeviceManager.h"
#include "VulkanLib.h"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanRenderPass {
 public:
  explicit VulkanRenderPass(Ref<VulkanDeviceManager> device_manager);
  ~VulkanRenderPass();

  static Ref<VulkanRenderPass> Create(Ref<VulkanDeviceManager> device_manager);
  VkRenderPass GetVkRenderPass() { return vk_render_pass_; }

 private:
  VulkanLib vklib_;
  VkRenderPass vk_render_pass_ = VK_NULL_HANDLE;
  Ref<VulkanDeviceManager> p_vulkan_device_manager_;
};

}// namespace aria
