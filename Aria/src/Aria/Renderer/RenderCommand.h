#pragma once
#include "RendererAPI.h"

namespace ARIA {
class RenderCommand {
 public:
  inline static void init() { sRendererAPI->init(); }

  inline static void set_clear_color(glm::vec4 color) { sRendererAPI->set_clear_color(color); }
  inline static void clear() { sRendererAPI->clear(); }
  inline static void draw_indexed(const Ref<VertexArray>& vertex_array) { sRendererAPI->draw_indexed(vertex_array); }

 private:
  static RendererAPI* sRendererAPI;
};
}  // namespace ARIA