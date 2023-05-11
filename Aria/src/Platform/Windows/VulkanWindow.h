#pragma once

#include "Aria/Core/Window.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_core.h>

namespace ARIA {
class VulkanWindow : public Window {
 public:
  VulkanWindow(const WindowProps& props);
  ~VulkanWindow() = default;

  void on_update() override;

  inline unsigned int get_width() const override { return window_data.width; }
  inline unsigned int get_height() const override { return window_data.height; }

  // Window attributes
  inline void set_event_callback(const EventCallbackFn& callback) override;
  void set_vsync(bool enabled) override;
  bool is_vsync() const override { return window_data.vsync; }

  void* get_native_window() const override;

 private:
  void init();
  void create_surface(VkInstance& instance);
  void create_window();

  GLFWwindow* glfw_window;
  VkSurfaceKHR vk_surface;

  struct VulkanWindowData {
    std::string title;
    unsigned int width, height;
    bool vsync;

    EventCallbackFn event_callback;
  };

  VulkanWindowData window_data;
};
}  // namespace ARIA
