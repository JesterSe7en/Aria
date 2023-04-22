#pragma once

namespace Aria {


class RendererAPI {
 public:
  enum class API {
    // DirectX and Vulkan would be added later
    None = 0,
    OpenGL = 1,
    DirectX = 2,
    Vulkan = 3,
  };

 public:
  static API GetAPI() { return api; }

 private:
  static API api;
};
}  // namespace Aria