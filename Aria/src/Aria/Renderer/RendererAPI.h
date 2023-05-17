#pragma once

#include "Aria/Renderer/VertexArray.h"

#include <glm/glm.hpp>

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
  virtual void init() = 0;
  virtual void clear() = 0;
  virtual void set_clear_color(const glm::vec4 color) = 0;
  virtual void draw_indexed(const Ref<VertexArray>& array) = 0;

  static API get_api() { return sAPI; }
  static void set_api(API api);
  static API create();

  virtual void create_pipeline() = 0;

 private:
  static API sAPI;
};
}  // namespace ARIA
