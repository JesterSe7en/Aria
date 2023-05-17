#pragma once
#include "Aria/Renderer/RendererAPI.h"
#include "RendererAPI.h"

#include <memory>

namespace ARIA {
class RenderCommand {
 public:
  static void init(RendererAPI::API api);

  inline static void set_clear_color(glm::vec4 color) { sRendererAPI->set_clear_color(color); }
  inline static void clear() { sRendererAPI->clear(); }
  inline static void draw_indexed(const Ref<VertexArray>& vertex_array) { sRendererAPI->draw_indexed(vertex_array); }

  static void create_pipeline();

 private:
  static std::unique_ptr<RendererAPI> sRendererAPI;
};
}  // namespace ARIA
