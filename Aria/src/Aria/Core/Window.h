#pragma once

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

  virtual const char** get_required_instance_extensions(uint32_t& count) const = 0;

  virtual void* get_native_window() const = 0;

  static Window* create(const WindowProps& props = WindowProps());
};
}  // namespace ARIA