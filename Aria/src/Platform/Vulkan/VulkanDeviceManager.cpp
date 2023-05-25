#include "ariapch.h"
#include "Platform/Vulkan/VulkanDeviceManager.hpp"
#include "Aria/Core/Log.hpp"
#include "Platform/Vulkan/VulkanRendererApi.hpp"
#include "vulkan/vk_enum_string_helper.h"
#include <cstring>
#include <set>

namespace aria {

VulkanDeviceManager::VulkanDeviceManager()
    : vk_instance_(VulkanRendererApi::GetInstance()->GetVulkanInstance()->GetVkInstance()),
      vk_surface_khr_(VulkanRendererApi::GetInstance()->GetVkSurfaceKhr()){
          ARIA_CORE_ASSERT(vk_instance_, "Did you initialize vk instance first?")
              ARIA_CORE_ASSERT(vk_surface_khr_, "Did you initialize presentation layer first?")
      }

VulkanDeviceManager::~VulkanDeviceManager() {
  vkDestroyDevice(device_, nullptr);
}

void VulkanDeviceManager::Init() {
  SelectSuitablePhysicalDevice();
  CreateLogicalDevice();
}

VulkanDeviceManager::PhysicalDeviceSurfaceSwapChainDetails VulkanDeviceManager::GetSwapChainSupportDetails() {
  return QuerySwapChainSupport(physical_device_.physical_device);
}

void VulkanDeviceManager::SelectSuitablePhysicalDevice() {
  std::uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vk_instance_, &device_count, nullptr);

  ARIA_CORE_ASSERT(device_count != 0, "You tried to setup with Vulkan API, but no GPU's found with Vulkan support")

  all_physical_devices_.resize(device_count);
  vkEnumeratePhysicalDevices(vk_instance_, &device_count, all_physical_devices_.data());

  for (VkPhysicalDevice physical_device : all_physical_devices_) {
    if (IsSuitableVulkanDevice(physical_device)) {
      physical_device_.physical_device = physical_device;
      physical_device_.queue_families = QueryQueueFamilies(physical_device_.physical_device);

      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(physical_device_.physical_device, &properties);

      ARIA_CORE_INFO("--- Vulkan GUI Device --- ")
      ARIA_CORE_INFO("Name: {0}", properties.deviceName)
      ARIA_CORE_INFO("Vulkan API version: {0}.{1}.{2}", VK_API_VERSION_MAJOR(properties.apiVersion),
                     VK_API_VERSION_MINOR(properties.apiVersion), VK_API_VERSION_PATCH(properties.apiVersion))

      // This may not work - vkspec v1.3 pg. 116
      // "The encoding of driverVersion is implementation-defined. It may not use the same
      // encoding as apiVersion. Applications should follow information from the vendor
      // on how to extract the version information from driverVersion."
      // TODO: May need to use this method
      //  check layers' description for human-readable driver version
      //  NVIDIA: VK_LAYER_NV_optimus
      //  AMD: VK_LAYER_AMD_switchable_graphics
      //  INTEL: VK_LAYER_KHRONOS_validation
      ARIA_CORE_INFO("Driver version: {0}.{1}.{2}", VK_VERSION_MAJOR(properties.driverVersion),
                     VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion))
      break;
    }
  }

  ARIA_CORE_ASSERT(physical_device_.physical_device != VK_NULL_HANDLE,
                   "Found GPUs with Vulkan support, but no suitable devices for Aria Engine")

  // For reference - Vendor IDs:
  //  case 0x10DE:
  //    return "NVIDIA";
  //  case 0x1002:
  //    return "AMD";
  //  case 0x8086:
  //    return "INTEL";

  // features can include optional capabilities such as geometry shaders, tessellation shaders, multi-viewport
  // rendering, texture compression formats, and more.

  // extensions
  // std::uint32_t extensions_count = 0;
  // vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensions_count, nullptr);

  // std::vector<VkExtensionProperties> extensions(extensions_count);
  // vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensions_count, extensions.data());

  // ARIA_CORE_INFO("Available extensions for physical device:")
  // for (const auto& extension : extensions) {
  //   ARIA_CORE_INFO(extension.extensionName)
  // }

  // to call device extensions funcs
  // vkGetDeviceProcAddr()

  // physical device layers
  // std::uint32_t layerCount;
  // vkEnumerateDeviceLayerProperties(sPhysicalDevice, &layerCount, nullptr);

  // std::vector<VkLayerProperties> availableLayers(layerCount);
  // vkEnumerateDeviceLayerProperties(sPhysicalDevice, &layerCount, availableLayers.data());
}

void VulkanDeviceManager::CreateLogicalDevice() {

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<std::uint32_t> unique_queue_families = {queue_family_indices_.graphics_family.value(),
                                                   queue_family_indices_.present_family.value()};

  float queue_priority = 1.0f;
  for (std::uint32_t queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
  create_info.pQueueCreateInfos = queue_create_infos.data();

  VkPhysicalDeviceFeatures device_features;
  create_info.pEnabledFeatures = &device_features;

  if (CheckDeviceExtensionsSupport(physical_device_.physical_device)) {
    create_info.ppEnabledExtensionNames = device_extensions_.data();
    create_info.enabledExtensionCount = static_cast<std::uint32_t>(device_extensions_.size());
  } else {
    create_info.enabledExtensionCount = 0;
  }

  if (VulkanRendererApi::GetInstance()->GetVulkanInstance()->AreValidationLayersEnabled()) {
    create_info.enabledLayerCount = static_cast<std::uint32_t>(VulkanRendererApi::kValidationLayers.size());
    create_info.ppEnabledLayerNames = VulkanRendererApi::kValidationLayers.data();
  } else {
    create_info.enabledLayerCount = 0;
  }

  VkResult result = vkCreateDevice(physical_device_.physical_device, &create_info, nullptr, &device_);
  if (result != VK_SUCCESS) { ARIA_CORE_ERROR("Cannot create logical device - {0}", string_VkResult(result)) }

  vkGetDeviceQueue(device_, queue_family_indices_.graphics_family.value(), 0, &graphics_queue_);
  vkGetDeviceQueue(device_, queue_family_indices_.present_family.value(), 0, &present_queue_);
}

std::vector<VkQueueFamilyProperties> VulkanDeviceManager::QueryQueueFamilies(VkPhysicalDevice &physical_device) {
  std::uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

  int i = 0;
  // All possible queue families
  // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html
  VkSurfaceKHR surface = VulkanRendererApi::GetInstance().GetVkSurfaceKhr();

  for (const auto &k_queue_family : queue_families) {
    VkBool32 surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &surface_supported);

    if (surface_supported) { queue_family_indices_.present_family = i; }

    if (k_queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) { queue_family_indices_.graphics_family = i; }

    if (queue_family_indices_.IsComplete()) { break; }
    i++;
  }
  return queue_families;
}

bool VulkanDeviceManager::IsSuitableVulkanDevice(VkPhysicalDevice &physical_device) {
  bool swap_chain_supported = false;
  if (CheckDeviceExtensionsSupport(physical_device)) {
    PhysicalDeviceSurfaceSwapChainDetails details = QuerySwapChainSupport(physical_device);
    swap_chain_supported = !details.formats.empty() && !details.present_modes.empty();
  }

  return queue_family_indices_.IsComplete() && swap_chain_supported;

  // VkPhysicalDeviceProperties properties;
  // vkGetPhysicalDeviceProperties(device, &properties);

  // VkPhysicalDeviceFeatures features;
  // vkGetPhysicalDeviceFeatures(device, &features);

  // as mentioned, can use scoring system to defer using Discrete GPU first
  // then integrated, or allow user to select
  // return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

VulkanDeviceManager::PhysicalDeviceSurfaceSwapChainDetails
VulkanDeviceManager::QuerySwapChainSupport(VkPhysicalDevice &physical_device) {
  VkSurfaceKHR surface = VulkanRendererApi::GetInstance().GetVkSurfaceKhr();
  PhysicalDeviceSurfaceSwapChainDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

  std::uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
  if (format_count) {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.formats.data());
  }

  std::uint32_t present_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_count, nullptr);
  if (present_count) {
    details.present_modes.resize(present_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_count, details.present_modes.data());
  }

  return details;
}

bool VulkanDeviceManager::CheckDeviceExtensionsSupport(VkPhysicalDevice &physical_device) {
  std::uint32_t count;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, nullptr);

  std::vector<VkExtensionProperties> extensions(count);
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, extensions.data());

  std::set<std::string> required_extensions(device_extensions_.begin(), device_extensions_.end());

  for (const auto &kExtension : extensions) { required_extensions.erase(kExtension.extensionName); }
  return required_extensions.empty();
}

}// namespace aria
