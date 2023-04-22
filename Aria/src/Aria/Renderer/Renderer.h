#pragma once

namespace Aria {

enum class RendererAPI {
  // DirectX and Vulkan would be added later
  None = 0,
  OpenGL = 1,
  DirectX = 2,
  Vulkan = 3,
};

class Renderer {
 public:
  inline static RendererAPI get_API() { return RENDERER_API; }
  inline static RendererAPI set_API(RendererAPI api) { RENDERER_API = api; }

 private:
  static RendererAPI RENDERER_API;
};
}  // namespace Aria