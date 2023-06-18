#pragma once

#include "VulkanInstance.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <optional>
#include <vector>

namespace aria {
class VulkanDeviceManager {

 public:
  explicit VulkanDeviceManager(Ref<VulkanInstance> vulkan_Instance);
  ~VulkanDeviceManager();

  static Ref<VulkanDeviceManager> Create(Ref<VulkanInstance> vulkan_instance);

  vkb::Device &GetLogicalDevice() { return logical_device_; }
  vkb::PhysicalDevice &GetPhysicalDevice() { return physical_device_; }
  vkb::Swapchain &GetSwapChain() { return swapchain_; }
  unsigned int GetQueueFamilyIndex() const;
  void RegenerateSwapchain();

 private:
  Ref<VulkanInstance> vulkan_instance_;
  vkb::PhysicalDevice physical_device_;
  vkb::Device logical_device_;
  vkb::Swapchain swapchain_;
  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};

  void SelectSuitablePhysicalDevice();
  void CreateLogicalDevice();
  void CreateSwapchain();
};
}// namespace aria
