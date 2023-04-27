#pragma once
#include "RendererAPI.h"

namespace ARIA {
class RenderCommand {
 public:
  inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) {
    sRendererAPI->draw_indexed(vertex_array);
  }
 private:
  static RendererAPI* sRendererAPI;
};
}  // namespace ARIA