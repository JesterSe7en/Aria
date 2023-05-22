#pragma once

#include "vulkan/vulkan_core.h"

class VulkanInstance {
 public:
  struct CreateInfo {
    bool enable_validation;
    uint32_t layer_count;
    std::vector<const char*> pp_layer_names;
    uint32_t extension_count;
    std::vector<const char*> pp_extension_names;
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
  };

  VulkanInstance(CreateInfo& create_info);

  bool GetInstanceExtensions(std::string_view layer_name, std::vector<VkExtensionProperties>& instance_extensions);
//  std::shared_ptr<VulkanInstance> Create
 private:
};