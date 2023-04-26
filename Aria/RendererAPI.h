#pragma once

#include <glm/glm.hpp>

#include <Aria/Renderer/VertexArray.h>

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
  virtual void Clear() = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;

  virtual void DrawIndexed(
      const std::shared_ptr<VertexArray>& vertex_array) = 0;

 private:
};
}  // namespace Aria