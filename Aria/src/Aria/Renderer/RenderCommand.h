#pragma once

#include "Aria/Renderer/RendererApi.h"
#include "RendererApi.h"

#include <memory>

namespace aria {
class RenderCommand {
 public:
  static void Init(RendererApi::Api api);

  inline static void SetClearColor(glm::vec4 color) { p_renderer_api_->SetClearColor(color); }
  inline static void Clear() { p_renderer_api_->Clear(); }
  inline static void DrawIndexed(const Ref<VertexArray> &vertex_array) { p_renderer_api_->DrawIndexed(vertex_array); }

  // Vulkan commands
  static void BeginRecording() { p_renderer_api_->BeginRecording(); }
  static void EndRecording() { p_renderer_api_->EndRecording(); }
  static void BeginRenderPass() { p_renderer_api_->BeginRenderPass(); }
  static void EndRenderPass() { p_renderer_api_->EndRenderPass(); }
  static void BindToGraphicsPipeline() { p_renderer_api_->BindToGraphicsPipeline(); }
  static void SetViewport() { p_renderer_api_->SetViewport(); }
  static void SetScissor() { p_renderer_api_->SetScissor(); }
  static void Draw() { p_renderer_api_->Draw(); }

 private:
  static std::unique_ptr<RendererApi> p_renderer_api_;
};
}// namespace aria
