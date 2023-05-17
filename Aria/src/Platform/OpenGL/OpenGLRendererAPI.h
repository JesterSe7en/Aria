#pragma once

#include "Aria/Renderer/RendererAPI.h"

#include <glm/glm.hpp>

namespace ARIA {
class OpenGLRendererAPI : public RendererAPI {
  void init() override;
  void clear() override;
  void set_clear_color(const glm::vec4 color) override;
  void draw_indexed(const Ref<VertexArray>& vertex_array) override;
  void create_pipeline() override {}
};
}  // namespace ARIA