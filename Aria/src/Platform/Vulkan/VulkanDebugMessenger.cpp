#include "ariapch.h"
#include "VulkanDebugMessenger.h"
#include "VkBootstrap.h"
#include "VulkanError.h"

namespace aria {

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger::VulkanLogCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, [[maybe_unused]] void *p_user_data) {
  switch (message_severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      ARIA_CORE_INFO("Vulkan {0} Info: {1}", vkb::to_string_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      ARIA_CORE_TRACE("Vulkan {0} Trace: {1}", vkb::to_string_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      ARIA_CORE_ERROR("Vulkan {0} Error: {1}", vkb::to_string_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      ARIA_CORE_WARN("Vulkan {0} Error: {1}", vkb::to_string_message_type(message_type), p_callback_data->pMessage)
      break;
    default:
      ARIA_CORE_ASSERT(false, "Unknown error type")
      break;
  }
  // per vulkan 1.3 spec, pg.3566
  // The callback returns a VkBool32, which is interpreted in a layer-specified manner. The application
  // should always return VK_FALSE. The VK_TRUE value is reserved for use in layer development.
  return VK_FALSE;
}

VkDebugUtilsMessageTypeFlagBitsEXT VulkanDebugMessenger::GetMessageType() {
  return static_cast<VkDebugUtilsMessageTypeFlagBitsEXT>(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);
}
VkDebugUtilsMessageSeverityFlagBitsEXT VulkanDebugMessenger::GetMessageSeverity() {
  return static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
}
}// namespace aria
