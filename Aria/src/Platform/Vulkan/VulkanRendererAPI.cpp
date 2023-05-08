#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Aria/Core/Application.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "ariapch.h"

#include "Aria/Renderer/VertexArray.h"

#include "VulkanRendererAPI.h"
#include "vulkan/vulkan_core.h"

#include <glad/gl.h>

namespace ARIA {
void VulkanRendererAPI::init() {
  ARIA_CORE_TRACE("hi from vulkan renderer api")
  create_instance();
}
void VulkanRendererAPI::clear() { ARIA_CORE_ASSERT(false, "Not Implemeented") }
void VulkanRendererAPI::set_clear_color(const glm::vec4 color) { ARIA_CORE_ASSERT(false, "Not Implemeented") }
void VulkanRendererAPI::draw_indexed(const Ref<VertexArray>& vertex_array){ARIA_CORE_ASSERT(false, "Not Implemeented")}

VkInstance VulkanRendererAPI::create_instance() {
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

  auto client_app = Application::get();
  auto glfw_window = client_app.get_window().get_native_window();
  //   uint32_t glfwExtensionCount = 0;
  // const char** glfwExtensions;

  // glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // createInfo.enabledExtensionCount = glfwExtensionCount;
  // createInfo.ppEnabledExtensionNames = glfwExtensions;
}

}  // namespace ARIA