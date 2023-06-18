#include "ariapch.h"
#include "Platform/Vulkan/VulkanDeviceManager.h"
#include "Aria/Core/Log.h"
#include "VkBootstrap.h"
#include "VulkanDeviceManager.h"
#include "VulkanError.h"
#include "VulkanLib.h"

namespace aria {

VulkanDeviceManager::VulkanDeviceManager(Ref<VulkanInstance> vulkan_instance) : vulkan_instance_(vulkan_instance) {
  SelectSuitablePhysicalDevice();
  CreateLogicalDevice();
  CreateSwapchain();
}

VulkanDeviceManager::~VulkanDeviceManager() {
  vkb::destroy_swapchain(swapchain_);
  vkb::destroy_device(logical_device_);
}

Ref<VulkanDeviceManager> VulkanDeviceManager::Create(Ref<VulkanInstance> vulkan_instance) {
  return std::make_shared<VulkanDeviceManager>(new VulkanDeviceManager(vulkan_instance));
}

void VulkanDeviceManager::SelectSuitablePhysicalDevice() {

  vkb::PhysicalDeviceSelector physical_device_selector(vulkan_instance_->GetVKBInstance());
  auto device_ret = physical_device_selector.set_surface(vulkan_instance_->GetVkSurface())
                        .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
                        .add_required_extensions(device_extensions_)
                        .select();
  ARIA_VKB_CHECK_RESULT_AND_ERROR(device_ret, "Failed to select suitable physical device")

  if (device_ret.has_value()) {
    ARIA_CORE_INFO("Found suitable physical device: {0}", device_ret.value().name);
    physical_device_ = device_ret.value();

    ARIA_CORE_INFO("--- Vulkan GPU Device --- ")
    ARIA_CORE_INFO("Name: {0}", device_ret.value().name)
    ARIA_CORE_INFO("Vulkan API version: {0}.{1}.{2}", VK_API_VERSION_MAJOR(physical_device_.properties.apiVersion),
                   VK_API_VERSION_MINOR(physical_device_.properties.apiVersion),
                   VK_API_VERSION_PATCH(physical_device_.properties.apiVersion))
  }

  //  std::uint32_t device_count = 0;
  //  vkEnumeratePhysicalDevices(vk_instance_, &device_count, nullptr);
  //
  //  ARIA_CORE_ASSERT(device_count != 0, "You tried to setup with Vulkan API, but no GPU's found with Vulkan support")
  //
  //  all_physical_devices_.resize(device_count);
  //  vkEnumeratePhysicalDevices(vk_instance_, &device_count, all_physical_devices_.data());
  //
  //  for (VkPhysicalDevice physical_device : all_physical_devices_) {
  //    if (IsSuitableVulkanDevice(physical_device)) {
  //      physical_device_.physical_device = physical_device;
  //      physical_device_.queue_families = QueryQueueFamilies(physical_device_.physical_device);
  //
  //      VkPhysicalDeviceProperties properties;
  //      vkGetPhysicalDeviceProperties(physical_device_.physical_device, &properties);
  //
  //      ARIA_CORE_INFO("--- Vulkan GUI Device --- ")
  //      ARIA_CORE_INFO("Name: {0}", properties.deviceName)
  //      ARIA_CORE_INFO("Vulkan API version: {0}.{1}.{2}", VK_API_VERSION_MAJOR(properties.apiVersion),
  //                     VK_API_VERSION_MINOR(properties.apiVersion), VK_API_VERSION_PATCH(properties.apiVersion))
  //
  //      // This may not work - vkspec v1.3 pg. 116
  //      // "The encoding of driverVersion is implementation-defined. It may not use the same
  //      // encoding as apiVersion. Applications should follow information from the vendor
  //      // on how to extract the version information from driverVersion."
  //      // TODO: May need to use this method
  //      //  check layers' description for human-readable driver version
  //      //  NVIDIA: VK_LAYER_NV_optimus
  //      //  AMD: VK_LAYER_AMD_switchable_graphics
  //      //  INTEL: VK_LAYER_KHRONOS_validation
  //      ARIA_CORE_INFO("Driver version: {0}.{1}.{2}", VK_VERSION_MAJOR(properties.driverVersion),
  //                     VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion))
  //      break;
  //    }
  //  }

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
  vkb::DeviceBuilder device_builder(physical_device_);
  auto device_ret = device_builder.build();
  ARIA_VKB_CHECK_RESULT_AND_ERROR(device_ret, "Failed to create logical device")
  if (device_ret.has_value()) {
    logical_device_ = device_ret.value();
    VulkanLib::GetInstance().InitDeviceFunctions(logical_device_);
  }
}

void VulkanDeviceManager::CreateSwapchain() {
  vkb::SwapchainBuilder swapchain_builder{logical_device_};

  // from vkb documentation...
  //  By default, the swapchain will use the VK_FORMAT_B8G8R8A8_SRGB or VK_FORMAT_R8G8B8A8_SRGB image format with the
  //  color space VK_COLOR_SPACE_SRGB_NONLINEAR_KHR. The present mode will default to VK_PRESENT_MODE_MAILBOX_KHR if
  //  available and fallback to VK_PRESENT_MODE_FIFO_KHR. The image usage
  //  default flag is VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT.
  auto swapchain_ret = swapchain_builder.build();
  ARIA_VKB_CHECK_RESULT_AND_ERROR(swapchain_ret, "Failed to create swapchain")
  if (swapchain_ret.has_value()) { swapchain_ = swapchain_ret.value(); }
}

unsigned int VulkanDeviceManager::GetQueueFamilyIndex() const {
  auto res = logical_device_.get_queue_index(vkb::QueueType::graphics);
  ARIA_VKB_CHECK_RESULT_AND_ASSERT(res, "Failed to get queue family index")
  return res.has_value() ? res.value() : 0;
}

void VulkanDeviceManager::RegenerateSwapchain() {}

}// namespace aria
