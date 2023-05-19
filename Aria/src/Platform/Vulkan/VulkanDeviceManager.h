#pragma once

#include "vulkan/vulkan_core.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace ARIA {
class VulkanDeviceManager {
 public:
  VulkanDeviceManager();

  static VulkanDeviceManager& get_instance() {
    static VulkanDeviceManager instance;
    return instance;
  }

 private:
  struct QueueFamilyIndices {
    std::optional<std::uint32_t> graphicsFamily;
    std::optional<std::uint32_t> presentFamily;
    bool is_complete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };

 public:
  struct VulkanPhysicalDevice {
    VkPhysicalDevice physicalDevice;
    std::vector<VkQueueFamilyProperties> queueFamilies;
  };

  VulkanDeviceManager(const VulkanDeviceManager&) = delete;
  VulkanDeviceManager& operator=(const VulkanDeviceManager&) = delete;

  static VkDevice get_logical_device();
  static VulkanPhysicalDevice get_physical_device() { return sPhysicalDevice; }

 private:
  // only one for now..yeah??
  static VkDevice sDevice;
  static VulkanPhysicalDevice sPhysicalDevice;
  static QueueFamilyIndices mQueueFamilies;

  VkInstance& mInstance;

  std::vector<VkPhysicalDevice> mAllPhysicalDevices;

  void selectSuitablePhysicalDevice();
  void createLogicalDevice();
  std::vector<VkQueueFamilyProperties> queryQueueFamilies(VkPhysicalDevice& physicalDevice);
  bool isSuitableVulkanDevice(VkPhysicalDevice& physicalDevice);
};
}  // namespace ARIA