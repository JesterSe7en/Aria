#pragma once

#include "Aria/Core/Base.h"
#include "VkBootstrap.h"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanInstance {
 public:
  struct VulkanInstanceCreateInfo {
    bool enable_validation;
    std::vector<const char *> pp_layer_names;
    std::vector<const char *> pp_extension_names;
  };

  ~VulkanInstance() = default;
  VulkanInstance(const VulkanInstance &) = delete;
  VulkanInstance(VulkanInstance &&) = delete;
  VulkanInstance &operator=(const VulkanInstance &) = delete;
  VulkanInstance &operator=(VulkanInstance &&) = delete;

  static Ref<VulkanInstance> Create(VulkanInstanceCreateInfo &create_info);

  vkb::Instance GetVKBInstance() { return vkb_instance_; }
  VkSurfaceKHR GetVkSurface() { return vk_surface_; }

 private:
  static uint32_t app_version_;
  static uint32_t engine_version_;
  static uint32_t api_version_;

  static std::vector<const char *> validation_layers_;

  vkb::Instance vkb_instance_;
  VkSurfaceKHR vk_surface_;
  std::vector<VkLayerProperties> available_layers_;
  std::vector<VkExtensionProperties> available_extensions_;

  explicit VulkanInstance(VulkanInstanceCreateInfo &create_info);
  void CreateWindowSurface();
};
}// namespace aria
