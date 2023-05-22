#pragma once

#include "Aria/Core/Base.h"
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
  VulkanInstance &operator=(const VulkanInstance &) = delete;

  static Ref<VulkanInstance> Create(VulkanInstanceCreateInfo &create_info);

  bool GetInstanceExtensions(const char *layer_name, std::vector<VkExtensionProperties> &instance_extensions);

 private:
  static uint32_t app_version_;
  static uint32_t engine_version_;
  static uint32_t api_version_;

  static std::vector<const char *> validation_layers_;

  std::vector<VkLayerProperties> available_layers_;

  bool IsLayerAvailable(const char* layer_name);
  void EnumerateLayerProperties();


  explicit VulkanInstance(VulkanInstanceCreateInfo &create_info);
  VkInstance vk_instance_ = VK_NULL_HANDLE;
};
}