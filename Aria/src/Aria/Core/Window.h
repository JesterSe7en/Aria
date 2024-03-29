#pragma once

#include "Aria/Events/Event.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/RendererApi.h"
#include "Base.h"

namespace aria {

struct WindowProps {
  std::string title;
  unsigned int width;
  unsigned int height;

  WindowProps(const std::string &title = "Aria Engine", unsigned int width = 1280, unsigned int height = 720)
      : title(title), width(width), height(height) {}
};

// Interface only meant for platforms with a desktop system
class ARIA_API Window {
 public:
  using EventCallbackFn = std::function<void(Event &)>;

  virtual void OnUpdate() = 0;

  virtual unsigned int GetWidth() const = 0;
  virtual unsigned int GetHeight() const = 0;

  // Window attributes
  virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void *GetNativeWindow() const = 0;

  virtual void CreateAriaWindow() = 0;

  static Window *Create(const WindowProps &props = WindowProps());
};
}// namespace aria
