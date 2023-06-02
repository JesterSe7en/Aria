#include "VulkanInstance.h"
#include "Aria/Core/Application.h"
#include "GLFW/glfw3.h"
#include "VulkanDebugMessenger.h"
#include "VulkanError.h"
#include "VulkanLib.h"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan/vulkan_core.h"

namespace aria {

uint32_t VulkanInstance::app_version_ = VK_MAKE_VERSION(1, 0, 0);
uint32_t VulkanInstance::engine_version_ = VK_MAKE_VERSION(1, 0, 0);
uint32_t VulkanInstance::api_version_ = VK_VERSION_1_3;

const std::vector<const char *> VulkanInstance::kValidationLayers = {"VK_LAYER_KHRONOS_validation"};

VulkanInstance::VulkanInstance(VulkanInstance::VulkanInstanceCreateInfo &create_info) : vk_surface_(VK_NULL_HANDLE) {

  auto sys_info_ret = vkb::SystemInfo::get_system_info();
  if (!sys_info_ret) {
    ARIA_CORE_ERROR("Failed to get system info - {0}", sys_info_ret.error().message());
    return;
  }
  vkb::SystemInfo &sys_info = sys_info_ret.value();

  vkb::InstanceBuilder instance_builder;
  instance_builder.set_app_name("Aria Vulkan Application")
      .set_app_version(app_version_)
      .set_engine_name("Aria")
      .set_engine_version(engine_version_)
      .require_api_version(api_version_);

  if (create_info.enable_validation) {
    instance_builder.enable_validation_layers(create_info.enable_validation);
    for (const char *layer_name : kValidationLayers) {
      if (sys_info.is_layer_available(layer_name)) { instance_builder.enable_layer(layer_name); }
    }

    instance_builder.set_debug_messenger_type(VulkanDebugMessenger::GetMessageType())
        .set_debug_messenger_severity(VulkanDebugMessenger::GetMessageSeverity())
        .set_debug_callback(VulkanDebugMessenger::VulkanLogCallback);
  }

  for (const char *extension_name : create_info.pp_extension_names) {
    if (sys_info.is_extension_available(extension_name)) { instance_builder.enable_extension(extension_name); }
  }

  for (const char *layer_name : create_info.pp_layer_names) {
    if (sys_info.is_layer_available(layer_name)) { instance_builder.enable_layer(layer_name); }
  }

  auto instance_ret = instance_builder.build();
  if (!instance_ret) { ARIA_CORE_ERROR("Failed to create Vulkan instance - {0}", instance_ret.error().message()); }
  vkb_instance_ = instance_ret.value();
}

Ref<VulkanInstance> VulkanInstance::Create(VulkanInstance::VulkanInstanceCreateInfo &create_info) {
  auto instance = new VulkanInstance(create_info);
  instance->CreateWindowSurface();
  VulkanLib::GetInstance().InitInstanceFunctions(instance->vkb_instance_);
  return Ref<VulkanInstance>(instance);
}

void VulkanInstance::CreateWindowSurface() {
  auto *window = (GLFWwindow *) Application::Get().GetWindow().GetNativeWindow();
  VkResult result = glfwCreateWindowSurface(vkb_instance_.instance, window, nullptr, &vk_surface_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create window surface")
}

}// namespace aria
