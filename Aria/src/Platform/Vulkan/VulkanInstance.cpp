#include "VulkanInstance.hpp"
#include "VulkanError.hpp"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan/vulkan_core.h"

namespace aria {

uint32_t VulkanInstance::app_version_ = VK_MAKE_VERSION(1, 0, 0);
uint32_t VulkanInstance::engine_version_ = VK_MAKE_VERSION(1, 0, 0);
uint32_t VulkanInstance::api_version_ = VK_VERSION_1_3;

std::vector<const char *> VulkanInstance::validation_layers_ = {"VK_LAYER_KHRONOS_validation"};

VulkanInstance::~VulkanInstance() { vkDestroyInstance(p_vk_instance_, nullptr); }

VulkanInstance::VulkanInstance(VulkanInstance::VulkanInstanceCreateInfo &create_info) {
  EnumerateLayerProperties();
  EnumerateInstanceExtensions();

  VkApplicationInfo app_info;
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Aria Vulkan Application";
  app_info.applicationVersion = app_version_;
  app_info.pEngineName = "Aria";
  app_info.engineVersion = engine_version_;
  app_info.apiVersion = api_version_;

  VkInstanceCreateInfo vk_instance_create_info;
  vk_instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vk_instance_create_info.flags = 0;
  vk_instance_create_info.pApplicationInfo = &app_info;

  if (create_info.enable_validation && AreLayersAvailable(validation_layers_)) {
    vk_instance_create_info.enabledLayerCount = create_info.layer_count + validation_layers_.size();

    std::vector<const char *> all_layers(create_info.layer_count + validation_layers_.size());
    all_layers.insert(create_info.pp_layer_names.begin(), create_info.pp_layer_names.begin(),
                      create_info.pp_layer_names.end());
    all_layers.insert(create_info.pp_layer_names.end(), validation_layers_.begin(), validation_layers_.end());
    vk_instance_create_info.ppEnabledLayerNames = all_layers.data();

    vk_instance_create_info.pNext = &create_info.debug_messenger_create_info;
    validation_enabled_ = true;
  } else {
    vk_instance_create_info.enabledLayerCount = 0;
    vk_instance_create_info.pNext = nullptr;
  }
  vk_instance_create_info.enabledExtensionCount = create_info.extension_count;
  vk_instance_create_info.ppEnabledExtensionNames = create_info.pp_extension_names.data();

  VkResult result = vkCreateInstance(&vk_instance_create_info, nullptr, &p_vk_instance_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create Vulkan instance")
}

Ref<VulkanInstance> VulkanInstance::Create(VulkanInstance::VulkanInstanceCreateInfo &create_info) {
  auto instance = new VulkanInstance(create_info);
  return Ref<VulkanInstance>(instance);
}

bool VulkanInstance::IsLayerAvailable(const char *layer_name) {
  for (const auto &kLayerName : available_layers_) {
    if (strcmp(kLayerName.layerName, layer_name) == 0) { return true; };
  }
  return false;
}

bool VulkanInstance::AreLayersAvailable(std::vector<const char *> layer_names) {
  return std::all_of(layer_names.begin(), layer_names.end(),
                     [this](const char *layer_name) { return IsLayerAvailable(layer_name); });
}

void VulkanInstance::EnumerateLayerProperties() {
  std::uint32_t layer_count;
  VkResult count_result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  ARIA_VK_CHECK_RESULT_AND_ERROR(count_result, "Failed to get instance layer count");

  if (layer_count != 0) {
    available_layers_.resize(layer_count);
    VkResult enum_result = vkEnumerateInstanceLayerProperties(&layer_count, available_layers_.data());
    ARIA_VK_CHECK_RESULT_AND_ERROR(enum_result, "Failed to enumerate instance layer properties");
  }
}

void VulkanInstance::EnumerateInstanceExtensions() {
  std::uint32_t extension_count = 0;
  VkResult count_result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
  ARIA_VK_CHECK_RESULT_AND_ERROR(count_result, "Failed to get instance extension count");

  if (extension_count != 0) {
    available_extensions_.resize(extension_count);
    VkResult enum_result =
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions_.data());
    ARIA_VK_CHECK_RESULT_AND_ERROR(enum_result, "Failed to enumerate instance extension properties");
  }
}

}// namespace aria
