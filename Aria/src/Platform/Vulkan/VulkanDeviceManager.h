#pragma once

#include "vulkan/vulkan_core.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace aria {
class VulkanDeviceManager {
 public:
  struct VulkanPhysicalDevice {
    VkPhysicalDevice physical_device;
    std::vector<VkQueueFamilyProperties> queue_families;
  };
  struct PhysicalDeviceSurfaceSwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };
  struct QueueFamilyIndices {
    std::optional<std::uint32_t> graphics_family;
    std::optional<std::uint32_t> present_family;
    bool IsComplete() const { return graphics_family.has_value() && present_family.has_value(); }
  };

 public:
  ~VulkanDeviceManager();
  VulkanDeviceManager(const VulkanDeviceManager &) = delete;
  VulkanDeviceManager &operator=(const VulkanDeviceManager &) = delete;

  static VulkanDeviceManager &GetInstance() {
    static VulkanDeviceManager instance;
    return instance;
  }

  VkDevice &GetLogicalDevice() { return device_; }
  VulkanPhysicalDevice &GetPhysicalDevice() { return physical_device_; }

  void Init();
  PhysicalDeviceSurfaceSwapChainDetails GetSwapChainSupportDetails();
  QueueFamilyIndices &GetQueueFamilyIndicies() { return queue_family_indices_; }

 private:
  VulkanDeviceManager();

  // only one for now...yeah??
  VkDevice device_;
  VulkanPhysicalDevice physical_device_;
  QueueFamilyIndices queue_family_indices_;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};

  VkInstance &vk_instance_;
  VkSurfaceKHR &vk_surface_khr_;

  std::vector<VkPhysicalDevice> all_physical_devices_;
  VkQueue present_queue_;
  VkQueue graphics_queue_;

  void SelectSuitablePhysicalDevice();
  void CreateLogicalDevice();
  std::vector<VkQueueFamilyProperties> QueryQueueFamilies(VkPhysicalDevice &physical_device);
  bool IsSuitableVulkanDevice(VkPhysicalDevice &physical_device);
  PhysicalDeviceSurfaceSwapChainDetails QuerySwapChainSupport(VkPhysicalDevice &physical_device);
  bool CheckDeviceExtensionsSupport(VkPhysicalDevice &physical_device);
};
}// namespace aria