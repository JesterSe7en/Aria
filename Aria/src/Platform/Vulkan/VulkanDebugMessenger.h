#pragma once

#include "VulkanInstance.h"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanDebugMessenger {
 public:
  VulkanDebugMessenger() = default;
  ~VulkanDebugMessenger() = default;

  static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                          VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                          const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
                                                          [[maybe_unused]] void *p_user_data);

  static VkDebugUtilsMessageSeverityFlagBitsEXT GetMessageSeverity();
  static VkDebugUtilsMessageTypeFlagBitsEXT GetMessageType();
};

}// namespace aria
