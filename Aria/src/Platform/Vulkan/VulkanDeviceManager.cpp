
#include "Platform/Vulkan/VulkanDeviceManager.h"

#include "Aria/Core/Log.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "vulkan/vk_enum_string_helper.h"

#include <string.h>

namespace ARIA {

VulkanDeviceManager::VulkanPhysicalDevice sPhysicalDevice{VK_NULL_HANDLE, {}};

VulkanDeviceManager::VulkanDeviceManager() : mInstance(VulkanRendererAPI::get_vk_instance()) {
  selectSuitablePhysicalDevice();
  createLogicalDevice();
}

void VulkanDeviceManager::selectSuitablePhysicalDevice() {
  std::uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(mInstance, &device_count, nullptr);

  ARIA_CORE_ASSERT(device_count != 0, "You tried to setup with Vulkan API, but no GPU's found with Vulkan support")

  mAllPhysicalDevices.resize(device_count);
  vkEnumeratePhysicalDevices(mInstance, &device_count, mAllPhysicalDevices.data());

  for (VkPhysicalDevice physicalDevice : mAllPhysicalDevices) {
    if (isSuitableVulkanDevice(physicalDevice)) {
      sPhysicalDevice.physicalDevice = physicalDevice;
      sPhysicalDevice.queueFamilies = queryQueueFamilies(sPhysicalDevice.physicalDevice);

      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(sPhysicalDevice.physicalDevice, &properties);

      ARIA_CORE_INFO("--- Vulkan GUI Device --- ")
      ARIA_CORE_INFO("Name: {0}", properties.deviceName)
      ARIA_CORE_INFO("Driver version: {0}.{1}.{2}", VK_VERSION_MAJOR(properties.driverVersion),
                     VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion))
      break;
    }
  }

  ARIA_CORE_ASSERT(sPhysicalDevice.physicalDevice != VK_NULL_HANDLE,
                   "Found GPUs with Vulkan support, but no suitable devices for Aria Engine")

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

  // TODO: get driver version
  //  check layers' description for human-readable driver version
  //  NVIDIA: VK_LAYER_NV_optimus
  //  AMD: VK_LAYER_AMD_switchable_graphics
  //  INTEL: VK_LAYER_KHRONOS_validation
}

// void VulkanDeviceManager::createLogicalDevice() {
//   QueryFamilyIndices indices = queryQueueFamilies(sPhysicalDevice);

//   std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
//   std::set<std::uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

//   float queuePriority = 1.0f;
//   for (std::uint32_t queueFamily : uniqueQueueFamilies) {
//     VkDeviceQueueCreateInfo queue_create_info{};
//     queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//     queue_create_info.queueFamilyIndex = queueFamily;
//     queue_create_info.queueCount = 1;
//     queue_create_info.pQueuePriorities = &queuePriority;
//     queue_create_infos.push_back(queue_create_info);
//   }

//   VkDeviceCreateInfo create_info{};
//   create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//   create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
//   create_info.pQueueCreateInfos = queue_create_infos.data();

//   VkPhysicalDeviceFeatures deviceFeatures{};
//   create_info.pEnabledFeatures = &deviceFeatures;

//   if (check_device_extensions_support(sPhysicalDevice)) {
//     create_info.ppEnabledExtensionNames = mDeviceExtensions.data();
//     create_info.enabledExtensionCount = static_cast<std::uint32_t>(mDeviceExtensions.size());
//   } else {
//     create_info.enabledExtensionCount = 0;
//   }

//   if (enable_validation_layers) {
//     create_info.enabledLayerCount = static_cast<std::uint32_t>(mValidationLayers.size());
//     create_info.ppEnabledLayerNames = mValidationLayers.data();
//   } else {
//     create_info.enabledLayerCount = 0;
//   }

//   VkResult result = vkCreateDevice(sPhysicalDevice, &create_info, nullptr, &sDevice);
//   if (result != VK_SUCCESS) {
//     ARIA_CORE_ERROR("Cannot create logical device - {0}", string_VkResult(result))
//   }

//   // vkGetDeviceQueue(sDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
//   // vkGetDeviceQueue(sDevice, indices.presentFamily.value(), 0, &mPresentQueue);
// }

std::vector<VkQueueFamilyProperties> VulkanDeviceManager::queryQueueFamilies(VkPhysicalDevice& physicalDevice) {
  std::uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  // All possible queue families
  // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html

  for (const auto& queueFamily : queueFamilies) {
    VkBool32 surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, mSurface, &surface_supported);

    if (surface_supported) {
      mQueueFamilies.presentFamily = i;
    }

    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      mQueueFamilies.graphicsFamily = i;
    }

    if (mQueueFamilies.is_complete()) {
      break;
    }
    i++;
  }
}

bool VulkanDeviceManager::isSuitableVulkanDevice(VkPhysicalDevice device) {
  QueryFamilyIndices queue_families = queryQueueFamilies(device);

  bool swap_chain_supported = false;
  if (check_device_extensions_support(device)) {
    SwapChainDetails details = query_swap_chain_support(device);
    swap_chain_supported = !details.formats.empty() && !details.presentModes.empty();
  }

  return queue_families.is_complete() && swap_chain_supported;

  // VkPhysicalDeviceProperties properties;
  // vkGetPhysicalDeviceProperties(device, &properties);

  // VkPhysicalDeviceFeatures features;
  // vkGetPhysicalDeviceFeatures(device, &features);

  // as mentioned, can use scoring system to defer using Discrete GPU first
  // then integrated, or allow user to select
  // return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

}  // namespace ARIA