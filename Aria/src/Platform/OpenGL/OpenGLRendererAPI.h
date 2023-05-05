#pragma once

#include <glm/glm.hpp>

#include "Aria/Renderer/RendererAPI.h"

namespace ARIA {
class OpenGLRendererAPI : public RendererAPI {
  virtual void clear() override;
  virtual void set_clear_color(const glm::vec4 color) override;
  virtual void draw_indexed(
      const Ref<VertexArray>& vertex_array) override;
};
}  // namespace ARIA