#pragma once
#include "RendererApi.h"

namespace aria {
class RenderCommand {
 public:
  inline static void Init() { p_renderer_api_->Init(); }

  inline static void SetClearColor(glm::vec4 color) { p_renderer_api_->SetClearColor(color); }
  inline static void Clear() { p_renderer_api_->Clear(); }
  inline static void DrawIndexed(const Ref<VertexArray>& vertex_array) { p_renderer_api_->DrawIndexed(vertex_array); }

 private:
  static RendererApi* p_renderer_api_;
};
}  // namespace ARIA