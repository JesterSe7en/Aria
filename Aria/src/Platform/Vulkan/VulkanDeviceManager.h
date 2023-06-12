#pragma once

#include "VulkanInstance.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <optional>
#include <vector>

namespace aria {
class VulkanDeviceManager {

 public:
  ~VulkanDeviceManager();
  
  VulkanDeviceManager(const VulkanDeviceManager &) = delete;
  VulkanDeviceManager &operator=(const VulkanDeviceManager &) = delete;

  static VulkanDeviceManager &GetInstance() {
    static VulkanDeviceManager instance_;
    return instance_;
  }
  void Init(Ref<VulkanInstance> &vulkan_instance);

  vkb::Device &GetLogicalDevice() { return logical_device_; }
  vkb::PhysicalDevice &GetPhysicalDevice() { return physical_device_; }
  vkb::Swapchain &GetSwapChain() { return swapchain_; }
  unsigned int GetQueueFamilyIndex();
  void RegenerateSwapchain();

 private:
  VulkanDeviceManager() = default;

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
