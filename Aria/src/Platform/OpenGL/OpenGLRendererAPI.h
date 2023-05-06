#pragma once

#include <glm/glm.hpp>

#include "Aria/Renderer/RendererAPI.h"

namespace ARIA {
class OpenGLRendererAPI : public RendererAPI {
  void init() override;
  void clear() override;
  void set_clear_color(const glm::vec4 color) override;
  void draw_indexed(const Ref<VertexArray>& vertex_array) override;
};
}  // namespace ARIA