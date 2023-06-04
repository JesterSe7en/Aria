#pragma once

#include "Aria/Renderer/RendererApi.h"
#include <glm/glm.hpp>

namespace aria {
class OpenGlRendererApi : public RendererApi {
  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &p_vertex_array) override;
};
}// namespace aria
