#include "ariapch.h"
#include "VulkanWindow.h"

namespace ARIA {

static bool s_glfw_initalized = false;
static void glfw_error_callback(int error_code, const char* description){
    ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description)}

VulkanWindow::VulkanWindow(const WindowProps& props) {
  window_data.title = props.mTitle;
  window_data.height = props.mHeight;
  window_data.width = props.mWidth;
  init();
}

void VulkanWindow::on_update() { glfwPollEvents(); }

void VulkanWindow::set_vsync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  window_data.vsync = enabled;
}

void VulkanWindow::init() {
  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", window_data.title, window_data.width, window_data.height)

  if (s_glfw_initalized) {
    int success = glfwInit();
    ARIA_ASSERT(success, "Failed to initialize GLFW")
    glfwSetErrorCallback(glfw_error_callback);
    s_glfw_initalized = true;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void VulkanWindow::create_surface(VkInstance& instance) {
  if (glfwCreateWindowSurface(instance, glfw_window, nullptr, &vk_surface) != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create window surface for Vulkan")
  }
}
}  // namespace ARIA
