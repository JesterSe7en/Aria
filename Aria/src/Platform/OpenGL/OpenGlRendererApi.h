#pragma once

#include <glm/glm.hpp>

#include "Aria/Renderer/RendererApi.h"

namespace aria {
class OpenGlRendererApi : public RendererApi {
  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray>& vertex_array) override;
};
}  // namespace ARIA