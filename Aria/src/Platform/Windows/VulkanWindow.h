#pragma once

#include "Aria/Core/Window.h"
#include "Aria/Renderer/RendererApi.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__) || defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_core.h>

namespace aria {
class VulkanWindow : public Window {
 public:
  VulkanWindow(const WindowProps &props);
  virtual ~VulkanWindow() = default;

  void OnUpdate() override;

  inline unsigned int GetWidth() const override { return window_data_.width; }
  inline unsigned int GetHeight() const override { return window_data_.height; }

  // Window attributes
  inline void SetEventCallback(const EventCallbackFn &callback) override { window_data_.event_callback = callback; }
  void SetVSync(bool enabled) override;
  inline bool IsVSync() const override { return window_data_.vsync; }

  void CreateAriaWindow() override;
  void *GetNativeWindow() const override { return p_glfw_window_; }

 private:
  void Init();

  bool glfw_initialized_ = false;
  GLFWwindow *p_glfw_window_;

  struct VulkanWindowData {
    std::string title;
    unsigned int width;
    unsigned int height;
    bool vsync;

    EventCallbackFn event_callback;
  };

  VulkanWindowData window_data_;
};
}// namespace aria
