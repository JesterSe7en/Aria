#include "ariapch.h"
#include "Window.hpp"
#include "Aria/Renderer/RendererApi.hpp"
#include "Platform/Windows/VulkanWindow.hpp"
#include "Platform/Windows/WindowsWindow.hpp"

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
