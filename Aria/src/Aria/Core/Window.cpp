#include "Window.h"
#include "Aria/Renderer/RendererAPI.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Windows/VulkanWindow.h"

namespace ARIA {
Window* Window::create(const WindowProps& props) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::OpenGL:
      return new WindowsWindow(props);
    case RendererAPI::API::Vulkan:
      return new VulkanWindow(props);
    default:
      ARIA_CORE_ASSERT(false, "Unknown API, cannot create window")
      return nullptr;
  }
}
}  // namespace ARIA
