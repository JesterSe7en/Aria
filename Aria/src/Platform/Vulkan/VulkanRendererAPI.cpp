#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Aria/Core/Application.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "ariapch.h"

#include "Aria/Renderer/VertexArray.h"

#include "VulkanRendererAPI.h"
#include "vulkan/vulkan_core.h"

#include <glad/gl.h>
#include <stdint.h>

namespace ARIA {

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

void VulkanRendererAPI::init() {
  ARIA_CORE_TRACE("hi from vulkan renderer api")
  create_instance();
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

  Application& client_app = Application::get();
  uint32_t count;
  const char** glfw_extensions;
  glfw_extensions = client_app.get_window().get_required_instance_extensions(count);

  create_info.enabledExtensionCount = count;
  create_info.ppEnabledExtensionNames = glfw_extensions;
  create_info.enabledLayerCount = 0;

  if (enable_validation_layers) {
    create_info.enabledLayerCount = mValidationLayers.size();
    create_info.ppEnabledLayerNames = mValidationLayers.data();
  }

  if (vkCreateInstance(&create_info, nullptr, &mInstance) != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create vulkan instance")
  }
}

bool VulkanRendererAPI::has_validation_support() const {
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

// WIP -add the creation of debug message config struct
// https://vulkan-tutorial.com/code/02_validation_layers.cpp

}  // namespace ARIA