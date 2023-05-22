#include "VulkanInstance.h"

namespace aria {

uint32_t VulkanInstance::app_version_ = VK_MAKE_VERSION(1, 0, 0);
uint32_t VulkanInstance::engine_version_ = VK_MAKE_VERSION(1, 0, 0);
uint32_t VulkanInstance::api_version_ = VK_VERSION_1_3;

std::vector<const char *> VulkanInstance::validation_layers_ = {"VK_LAYER_KHRONOS_validation"};

VulkanInstance::VulkanInstance(VulkanInstance::VulkanInstanceCreateInfo &create_info) {

//  vkEnumerateInstanceLayerProperties()
//
//
//  VkApplicationInfo app_info;
//  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//  app_info.pApplicationName = "Aria Vulkan Application";
//  app_info.applicationVersion = app_version_;
//  app_info.pEngineName = "Aria";
//  app_info.engineVersion = engine_version_;
//  app_info.apiVersion = api_version_;
//
//  VkInstanceCreateInfo vk_instance_create_info;
//  vk_instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//  vk_instance_create_info.flags = 0;
//  vk_instance_create_info.pApplicationInfo = &app_info;

}

VulkanInstance::~VulkanInstance() {

}

bool VulkanInstance::GetInstanceExtensions(const char *layer_name,
                                           std::vector<VkExtensionProperties> &instance_extensions) {
  return false;
}


Ref<VulkanInstance> VulkanInstance::Create(VulkanInstance::VulkanInstanceCreateInfo &create_info) {
  auto instance = VulkanInstance(create_info);
  return Ref<VulkanInstance>{instance};
}

bool VulkanInstance::IsLayerAvailable(const char* layer_name) {

  for (const auto& layer_name: available_layers_) {
    if (strcmp(layer_name, ))
  }
  return false;
}

void  VulkanInstance::EnumerateLayerProperties() {
  std::uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  vkEnumerateInstanceLayerProperties(&layer_count, available_layers_.data());
}
}