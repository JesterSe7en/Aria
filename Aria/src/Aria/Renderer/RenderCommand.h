#pragma once
#include <memory>
#include "Aria/Renderer/RendererAPI.h"
#include "RendererAPI.h"

namespace ARIA {
class RenderCommand {
 public:
  static void init(RendererAPI::API api);

  inline static void set_clear_color(glm::vec4 color) { sRendererAPI->set_clear_color(color); }
  inline static void clear() { sRendererAPI->clear(); }
  inline static void draw_indexed(const Ref<VertexArray>& vertex_array) { sRendererAPI->draw_indexed(vertex_array); }
  static Ref<RendererAPI> get_api_instance();

 private:
  static Ref<RendererAPI> sRendererAPI;
};
}  // namespace ARIA