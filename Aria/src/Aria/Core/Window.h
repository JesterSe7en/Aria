#pragma once

#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/RendererAPI.h"
#include "ariapch.h"
#include "Base.h"
#include "Aria/Events/Event.h"

namespace ARIA {

struct WindowProps {
  std::string mTitle;
  unsigned int mWidth;
  unsigned int mHeight;

  WindowProps(const std::string& title = "Aria Engine", unsigned int width = 1280, unsigned int height = 720)
      : mTitle(title), mWidth(width), mHeight(height) {}
};

// Interface only meant for platforms with a desktop system
class ARIA_API Window {
 public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual void on_update() = 0;

  virtual unsigned int get_width() const = 0;
  virtual unsigned int get_height() const = 0;

  // Window attributes
  virtual void set_event_callback(const EventCallbackFn& callback) = 0;
  virtual void set_vsync(bool enabled) = 0;
  virtual bool is_vsync() const = 0;

  virtual void* get_native_window() const = 0;

  virtual void create_window() = 0;

  // TODO: I don't really like that Window's abstraction has these by themselves. Argubably, OpenGL could call these but
  // it isn't used
  virtual void create_surface() {
    // Vulkan ONLY
  }
  virtual const char** get_requred_instance_extenstions(uint32_t& count) {
    // Vulkan ONLY
    return nullptr;
  }

  static Window* create(const WindowProps& props = WindowProps());
};
}  // namespace ARIA