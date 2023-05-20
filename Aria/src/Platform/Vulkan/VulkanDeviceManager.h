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

 public:
  struct VulkanPhysicalDevice {
    VkPhysicalDevice physical_device;
    std::vector<VkQueueFamilyProperties> queue_families;
  };

  VulkanDeviceManager(const VulkanDeviceManager &) = delete;
  VulkanDeviceManager &operator=(const VulkanDeviceManager &) = delete;

  static VkDevice GetLogicalDevice();
  static VulkanPhysicalDevice GetPhysicalDevice() { return physical_device_; }

 private:
  // only one for now..yeah??
  static VkDevice device_;
  static VulkanPhysicalDevice physical_device_;
  static QueueFamilyIndices queue_family_indices_;

  VkInstance &instance_;

  std::vector<VkPhysicalDevice> all_physical_devices_;

  void SelectSuitablePhysicalDevice();
  void CreateLogicalDevice();
  std::vector<VkQueueFamilyProperties> QueryQueueFamilies(VkPhysicalDevice &physical_device);
  bool IsSuitableVulkanDevice(VkPhysicalDevice &physical_device);
};
}  // namespace ARIA