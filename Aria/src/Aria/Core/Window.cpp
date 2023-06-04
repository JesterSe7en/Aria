#include "ariapch.h"
#include "Window.h"
#include "Aria/Renderer/RendererApi.h"
#include "Platform/Windows/VulkanWindow.h"
#include "Platform/Windows/WindowsWindow.h"

namespace aria {
Window *Window::Create(const WindowProps &props) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::OPEN_GL:
      return new WindowsWindow(props);
    case RendererApi::Api::VULKAN:
      return new VulkanWindow(props);
    default:
      ARIA_CORE_ASSERT(false, "Unknown API, cannot create window")
      return nullptr;
  }
}
}// namespace aria
