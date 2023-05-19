#pragma once

#include "Aria/Core/Window.h"
#include "Aria/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace aria {

class WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void OnUpdate() override;

  inline unsigned int GetWidth() const override { return window_data_.width; }
  inline unsigned int GetHeight() const override { return window_data_.height; }

  // Window attributes
  inline void SetEventCallback(const EventCallbackFn& callback) override {
    window_data_.event_callback_fn = callback;
  }

  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  virtual void* GetNativeWindow() const override { return p_glfw_window_; }

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  GLFWwindow* p_glfw_window_;
  GraphicsContext* p_graphics_context_;

  struct WindowData {
    std::string title;
    unsigned int width, height;
    bool v_sync;

    EventCallbackFn event_callback_fn;
  };

  WindowData window_data_;
};
}  // namespace ARIA
