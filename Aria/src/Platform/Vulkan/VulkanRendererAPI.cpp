#include "ariapch.h"

#include "VulkanRendererAPI.h"

#include "Aria/Core/Application.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/RendererAPI.h"
#include "Aria/Renderer/VertexArray.h"
#include "GLFW/glfw3.h"
#include "vulkan/vulkan_core.h"

#include <fileapi.h>
#include <stdint.h>

#include <array>
#include <set>
#include <vector>

namespace ARIA {

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

VkInstance VulkanRendererAPI::sInstance = nullptr;
VkDevice VulkanRendererAPI::sDevice = VK_NULL_HANDLE;

VulkanRendererAPI::~VulkanRendererAPI() {
  // TODO: Need to fill this in with memory deallocation calls
}

void VulkanRendererAPI::init() {
  create_instance();
  setup_vulkan_debug_messenger();
  create_presentation_surface();
  pick_physical_device();
  create_logical_device();
}
void VulkanRendererAPI::clear() { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererAPI::set_clear_color(const glm::vec4 color) { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererAPI::draw_indexed(const Ref<VertexArray>& vertex_array) {
  ARIA_CORE_ASSERT(false, "Not Implemented")
}

void VulkanRendererAPI::create_instance() {
  if (enable_validation_layers && !has_validation_support()) {
    ARIA_CORE_WARN("Vulkan validation layers requested, but none available")
  }

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Aria Vulkan Application";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Aria";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_VERSION_1_3;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  std::vector<const char*> extensions = get_glfw_required_extensions();

  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.enabledLayerCount = 0;

  // setup another create info struct to capture events during creation and destruction of VKInstance
  // Vulkan 1.3 spec pg. 3921
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
  if (enable_validation_layers) {
    create_info.enabledLayerCount = mValidationLayers.size();
    create_info.ppEnabledLayerNames = mValidationLayers.data();
    populate_debug_create_info(debug_create_info);
    create_info.pNext = &debug_create_info;
  } else {
    create_info.enabledLayerCount = 0;
    create_info.pNext = nullptr;
  }

  // query the supported instance extensions
  // uint32_t numInstanceExtensions = 0;
  // std::vector<VkExtensionProperties> instanceExtensionProperties;
  // Query the instance extensions.
  // vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, nullptr);

  // If there are any extensions, query their properties.

  // if (numInstanceExtensions != 0) {
  // instanceExtensionProperties.resize(numInstanceExtensions);
  // vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, instanceExtensionProperties.data());
  // }
  // if instance extensions are enabled, look for function pointers via
  // vkGetInstanceProcAddr() func

  if (vkCreateInstance(&create_info, nullptr, &sInstance) != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create vulkan instance")
  }
}

void VulkanRendererAPI::setup_vulkan_debug_messenger() {
  if (!enable_validation_layers) {
    return;
  }
  ARIA_CORE_ASSERT(sInstance != nullptr, "Did you create VkInstance before setting up debug messenger?")

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  populate_debug_create_info(create_info);

  if (create_debug_util_messenger_ext(sInstance, &create_info, nullptr, &mDebugMessenger) != VK_SUCCESS) {
    ARIA_CORE_WARN("Cannot setup debug messenger; debug messenger extention not available")
  }
}

void VulkanRendererAPI::create_presentation_surface() {
  ARIA_CORE_ASSERT(sInstance, "Did you create VkInstance first?")
  auto glfw_window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
  ARIA_CORE_ASSERT(glfw_window, "Did you create window first before creating surface?")
  if (glfwCreateWindowSurface(sInstance, glfw_window, nullptr, &mSurface) != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create vulkan surface")
  }
}

void VulkanRendererAPI::pick_physical_device() {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(sInstance, &device_count, nullptr);

  // TODO: maybe error out completely?
  ARIA_CORE_ASSERT(device_count != 0, "You tried to setup with Vulkan API, but no GPU's found with Vulkan support")

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(sInstance, &device_count, devices.data());

  for (const auto& device : devices) {
    if (is_suitable_vulkan_device(device)) {
      mPhysicalDevice = device;
      break;
    }
  }

  ARIA_CORE_ASSERT(mPhysicalDevice != VK_NULL_HANDLE,
                   "Found GPUs with Vulkan support, but no suitable devices for Aria Engine")

  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);

  ARIA_CORE_INFO("--- Vulkan GUI Device --- ")
  ARIA_CORE_INFO("Name: {0}", properties.deviceName)
  ARIA_CORE_INFO("Driver version: {0}", properties.driverVersion)

  // features can include optional capabilities such as geometry shaders, tessellation shaders, multi-viewport
  // rendering, texture compression formats, and more.

  // extensions
  uint32_t extensions_count = 0;
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensions_count, nullptr);

  std::vector<VkExtensionProperties> extensions(extensions_count);
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensions_count, extensions.data());

  ARIA_CORE_INFO("Available extensions for physical device:")
  for (const auto& extension : extensions) {
    ARIA_CORE_INFO(extension.extensionName)
  }

  // to call device extensions funcs
  // vkGetDeviceProcAddr()

  // physical device layers
  uint32_t layerCount;
  vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, availableLayers.data());
  // TODO: get driver version
  //  check layers' description for human-readable driver version
  //  NVIDIA: VK_LAYER_NV_optimus
  //  AMD: VK_LAYER_AMD_switchable_graphics
  //  INTEL: VK_LAYER_KHRONOS_validation
}

void VulkanRendererAPI::create_logical_device() {
  QueryFamilyIndicies indices = query_queue_families(mPhysicalDevice);

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queueFamily;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queuePriority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  create_info.pQueueCreateInfos = queue_create_infos.data();

  VkPhysicalDeviceFeatures deviceFeatures{};
  create_info.pEnabledFeatures = &deviceFeatures;

  if (check_device_extensions_support(mPhysicalDevice)) {
    create_info.ppEnabledExtensionNames = mDeviceExtensions.data();
    create_info.enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size());
  } else {
    create_info.enabledExtensionCount = 0;
  }

  if (enable_validation_layers) {
    create_info.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
    create_info.ppEnabledLayerNames = mValidationLayers.data();
  } else {
    create_info.enabledLayerCount = 0;
  }

  if (vkCreateDevice(mPhysicalDevice, &create_info, nullptr, &sDevice) != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create logical device")
  }

  vkGetDeviceQueue(sDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
  vkGetDeviceQueue(sDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}

bool VulkanRendererAPI::has_validation_support() const {
  // how many instance layers can the vulkan system support?
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char* layerName : mValidationLayers) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }
  return true;
}

void VulkanRendererAPI::populate_debug_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info) const {
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = vulkan_log_callback;
}

VkResult VulkanRendererAPI::create_debug_util_messenger_ext(VkInstance instance,
                                                            const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
                                                            const VkAllocationCallbacks* p_allocator,
                                                            VkDebugUtilsMessengerEXT* p_debug_messenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRendererAPI::vulkan_log_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data) {
  switch (message_severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      ARIA_CORE_INFO("Vulkan {0} Info: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      ARIA_CORE_TRACE("Vulkan {0} Trace: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      ARIA_CORE_ERROR("Vulkan {0} Error: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      ARIA_CORE_WARN("Vulkan {0} Error: {1}", get_message_type(message_type), p_callback_data->pMessage)
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

std::string VulkanRendererAPI::get_message_type(VkDebugUtilsMessageTypeFlagsEXT message_type) {
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
      return "Unknown Message Type";
  }
}

bool VulkanRendererAPI::is_suitable_vulkan_device(VkPhysicalDevice device) {
  QueryFamilyIndicies queue_families = query_queue_families(device);

  return queue_families.is_complete();

  // VkPhysicalDeviceProperties properties;
  // vkGetPhysicalDeviceProperties(device, &properties);

  // VkPhysicalDeviceFeatures features;
  // vkGetPhysicalDeviceFeatures(device, &features);

  // as mentioned, can use scoring system to defer using Discrete GPU first
  // then integrated, or allow user to select
  // return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

std::string VulkanRendererAPI::get_vendor_name(uint32_t vendor_id) const {
  switch (vendor_id) {
    case 0x10DE:
      return "NVIDIA";
    case 0x1002:
      return "AMD";
    case 0x8086:
      return "INTEL";
    default:
      return "Unknown";
  }
}

VulkanRendererAPI::QueryFamilyIndicies VulkanRendererAPI::query_queue_families(VkPhysicalDevice device) {
  QueryFamilyIndicies indicies;

  uint32_t query_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &query_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> query_families(query_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &query_family_count, query_families.data());

  int i = 0;
  // All possible queue families
  // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html

  for (const auto& queueFamily : query_families) {
    VkBool32 surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &surface_supported);

    if (surface_supported) {
      indicies.presentFamily = i;
    }

    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indicies.graphicsFamily = i;
    }

    if (indicies.is_complete()) {
      break;
    }
    i++;
  }
  return indicies;
}

VulkanRendererAPI::SwapChainDetails VulkanRendererAPI::query_swap_chain_support(VkPhysicalDevice device) {
  SwapChainDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, mSurface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, nullptr);

  if (formatCount) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, details.formats.data());
  }

  return details;
}

std::vector<const char*> VulkanRendererAPI::get_glfw_required_extensions() {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (enable_validation_layers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool VulkanRendererAPI::check_device_extensions_support(VkPhysicalDevice device) {
  uint32_t count;
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &count, nullptr);

  std::vector<VkExtensionProperties> extensions(count);
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &count, extensions.data());

  std::set<std::string> required_extensions(mDeviceExtensions.begin(), mDeviceExtensions.end());

  for (const auto& extension : extensions) {
    required_extensions.erase(extension.extensionName);
  }
  return required_extensions.empty();
}

}  // namespace ARIA
