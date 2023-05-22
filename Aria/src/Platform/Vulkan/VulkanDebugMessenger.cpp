#include "ariapch.h"
#include "VulkanDebugMessenger.h"
#include "VulkanRendererApi.h"
namespace aria {

VkDebugUtilsMessengerCreateInfoEXT VulkanDebugMessenger::debug_utils_messenger_create_info_ext_ = {};

VulkanDebugMessenger::~VulkanDebugMessenger() {
  if (VulkanRendererApi::IsValidationLayersEnabled()) {
    auto func =
        (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(VulkanRendererApi::GetInstance().GetVkInstance(),
                                                                    "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(VulkanRendererApi::GetInstance().GetVkInstance(), debug_messenger_, nullptr);
    }
  }
}

void VulkanDebugMessenger::Init() {
  SetupVulkanDebugMessenger();
}

std::string VulkanDebugMessenger::GetMessageType(VkDebugUtilsMessageTypeFlagsEXT message_type) {
  switch (message_type) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
      return "General";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
      return "Performance";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
      return "Validation";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
      return "Device Address";
    default:
      return "Unknown message type";
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger::VulkanLogCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data) {
  switch (message_severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      ARIA_CORE_INFO("Vulkan {0} Info: {1}", GetMessageType(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      ARIA_CORE_TRACE("Vulkan {0} Trace: {1}", GetMessageType(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      ARIA_CORE_ERROR("Vulkan {0} Error: {1}", GetMessageType(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      ARIA_CORE_WARN("Vulkan {0} Error: {1}", GetMessageType(message_type), p_callback_data->pMessage)
      break;
    default: ARIA_CORE_ASSERT(false, "Unknown error type")
      break;
  }
  // per vulkan 1.3 spec, pg.3566
  // The callback returns a VkBool32, which is interpreted in a layer-specified manner. The application
  // should always return VK_FALSE. The VK_TRUE value is reserved for use in layer development.
  return VK_FALSE;
}

void VulkanDebugMessenger::PopulateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info) {
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = VulkanLogCallback;
}

VkResult VulkanDebugMessenger::CreateDebugUtilMessengerExt(VkInstance instance,
                                                           const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
                                                           const VkAllocationCallbacks *p_allocator,
                                                           VkDebugUtilsMessengerEXT *p_debug_messenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanDebugMessenger::SetupVulkanDebugMessenger() {
  if (!VulkanRendererApi::IsValidationLayersEnabled()) {
    return;
  }

  ARIA_CORE_ASSERT(VulkanRendererApi::GetInstance().GetVkInstance() != nullptr,
                   "Did you create VkInstance before setting up debug messenger?")

  PopulateDebugCreateInfo(debug_utils_messenger_create_info_ext_);

  if (CreateDebugUtilMessengerExt(VulkanRendererApi::GetInstance().GetVkInstance(),
                                  &debug_utils_messenger_create_info_ext_,
                                  nullptr,
                                  &debug_messenger_)
      != VK_SUCCESS) {
    ARIA_CORE_WARN("Cannot setup debug messenger; debug messenger extension not available")
  }
}

} // namespace aria