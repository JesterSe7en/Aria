#pragma once

#include "Aria/Core/Base.hpp"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanInstance {
 public:
  struct VulkanInstanceCreateInfo {
    bool enable_validation;
    uint32_t layer_count;
    std::vector<const char *> pp_layer_names;
    uint32_t extension_count;
    std::vector<const char *> pp_extension_names;
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
  };

  ~VulkanInstance();
  VulkanInstance(const VulkanInstance &) = delete;
  VulkanInstance(VulkanInstance &&) = delete;
  VulkanInstance &operator=(const VulkanInstance &) = delete;
  VulkanInstance &operator=(VulkanInstance &&) = delete;

  static Ref<VulkanInstance> Create(VulkanInstanceCreateInfo &create_info);

  VkInstance GetVkInstance() { return p_vk_instance_; }
  bool IsLayerAvailable(const char *layer_name);
  bool AreLayersAvailable(std::vector<const char *> layer_names);
  bool AreValidationLayersEnabled() const { return validation_enabled_; }

 private:
  static uint32_t app_version_;
  static uint32_t engine_version_;
  static uint32_t api_version_;

  static std::vector<const char *> validation_layers_;

  VkInstance p_vk_instance_ = VK_NULL_HANDLE;
  bool validation_enabled_ = false;
  std::vector<VkLayerProperties> available_layers_;
  std::vector<VkExtensionProperties> available_extensions_;

  explicit VulkanInstance(VulkanInstanceCreateInfo &create_info);
  void EnumerateLayerProperties();
  void EnumerateInstanceExtensions();
};
}// namespace aria
