#pragma once

#include "VulkanRendererApi.h"

namespace aria {

class VulkanDebugMessenger {
 public:
  VulkanDebugMessenger();
  ~VulkanDebugMessenger();
 private:
  VkDebugUtilsMessengerEXT debug_messenger_;

  static std::string GetMessageType(VkDebugUtilsMessageTypeFlagsEXT message_type);
  static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLogCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
      const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data);

  void PopulateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info) const;
  VkResult CreateDebugUtilMessengerExt(VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
                                       const VkAllocationCallbacks *p_allocator,
                                       VkDebugUtilsMessengerEXT *p_debug_messenger);
  void SetupVulkanDebugMessenger();
};

} // namespace aria

