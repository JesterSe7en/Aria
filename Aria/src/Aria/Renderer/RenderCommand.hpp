#pragma once

#include "Aria/Renderer/RendererApi.hpp"
#include "RendererApi.hpp"

#include <memory>

namespace aria {
class RenderCommand {
 public:
  static void Init(RendererApi::Api api);

  inline static void SetClearColor(glm::vec4 color) { p_renderer_api_->SetClearColor(color); }
  inline static void Clear() { p_renderer_api_->Clear(); }
  inline static void DrawIndexed(const Ref<VertexArray> &vertex_array) { p_renderer_api_->DrawIndexed(vertex_array); }

  static void CreatePipeline();

 private:
  static std::unique_ptr<RendererApi> p_renderer_api_;
};
}// namespace aria
