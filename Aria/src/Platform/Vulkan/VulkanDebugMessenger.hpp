#pragma once

#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanDebugMessenger {
 public:
  VulkanDebugMessenger() = default;
  ~VulkanDebugMessenger();

  void Init();
  static VkDebugUtilsMessengerCreateInfoEXT &GetDebugMessengerCreateInfo() {
    return debug_utils_messenger_create_info_ext_;
  }

 private:
  VkDebugUtilsMessengerEXT debug_messenger_;
  static VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info_ext_;

  static std::string GetMessageType(VkDebugUtilsMessageTypeFlagsEXT message_type);
  static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                          VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                          const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
                                                          void *p_user_data);
  static void PopulateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info);

  static VkResult CreateDebugUtilMessengerExt(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
                                              const VkAllocationCallbacks *p_allocator,
                                              VkDebugUtilsMessengerEXT *p_debug_messenger);
  void SetupVulkanDebugMessenger();
};

}// namespace aria
