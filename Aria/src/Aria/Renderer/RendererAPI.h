#pragma once

#include <glm/glm.hpp>
#include "Aria/Renderer/VertexArray.h"

namespace ARIA {
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
  virtual void clear() = 0;
  virtual void set_clear_color(const glm::vec4 color) = 0;
  virtual void draw_indexed(const std::shared_ptr<VertexArray>& array) = 0;

  inline static API get_api() { return sAPI; }

 private:
  static API sAPI;
};
}  // namespace Aria