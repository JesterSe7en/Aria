#pragma once

#include "vulkan/vulkan_core.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace aria {
class VulkanDeviceManager {
 public:
  VulkanDeviceManager();

  static VulkanDeviceManager &GetInstance() {
    static VulkanDeviceManager instance;
    return instance;
  }

 private:
  struct QueueFamilyIndices {
    std::optional<std::uint32_t> graphics_family;
    std::optional<std::uint32_t> present_family;
    bool IsComplete() const { return graphics_family.has_value() && present_family.has_value(); }
  };

  struct SwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

 public:
  struct VulkanPhysicalDevice {
    VkPhysicalDevice physical_device;
    std::vector<VkQueueFamilyProperties> queue_families;
  };

  VulkanDeviceManager(const VulkanDeviceManager &) = delete;
  VulkanDeviceManager &operator=(const VulkanDeviceManager &) = delete;

  static VkDevice GetLogicalDevice() { return device_; }
  static VulkanPhysicalDevice GetPhysicalDevice() { return physical_device_; }

 private:
  // only one for now...yeah??
  static VkDevice device_;
  static VulkanPhysicalDevice physical_device_;
  static QueueFamilyIndices queue_family_indices_;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};

  VkInstance &instance_;

  std::vector<VkPhysicalDevice> all_physical_devices_;

  void SelectSuitablePhysicalDevice();
  void CreateLogicalDevice();
  std::vector<VkQueueFamilyProperties> QueryQueueFamilies(VkPhysicalDevice &physical_device);
  bool IsSuitableVulkanDevice(VkPhysicalDevice &physical_device);
  SwapChainDetails QuerySwapChainSupport(VkPhysicalDevice &physical_device);
  bool CheckDeviceExtensionsSupport(VkPhysicalDevice &physical_device);
};
}// namespace aria