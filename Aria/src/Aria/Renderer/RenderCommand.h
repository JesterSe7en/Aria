#pragma once

#include "Aria/Renderer/RendererApi.h"
#include "RendererApi.h"

#include <memory>

namespace aria {
class RenderCommand {
 public:
  static Scope<RendererApi> Init(RendererApi::Api api);

  inline static void SetClearColor(glm::vec4 color) { p_renderer_api_->SetClearColor(color); }
  inline static void Clear() { p_renderer_api_->Clear(); }
  inline static void DrawIndexed(const Ref<VertexArray> &vertex_array) { p_renderer_api_->DrawIndexed(vertex_array); }

  // Vulkan commands
  static void CreateCommandModule() { p_renderer_api_->CreateCommandModule(); }
  static void BeginRecording() { p_renderer_api_->BeginRecording(); }
  static void EndRecording() { p_renderer_api_->EndRecording(); }
  static void CmdBeginRenderPass() { p_renderer_api_->CmdBeginRenderPass(); }
  static void CmdEndRenderPass() { p_renderer_api_->CmdEndRenderPass(); }
  static void CmdBindToGraphicsPipeline() { p_renderer_api_->CmdBindToGraphicsPipeline(); }
  static void CmdSetViewport() { p_renderer_api_->CmdSetViewport(); }
  static void CmdSetScissor() { p_renderer_api_->CmdSetScissor(); }
  static void CmdDraw() { p_renderer_api_->CmdDraw(); }
  static void DrawFrame() { p_renderer_api_->DrawFrame(); }

 private:
  static Scope<RendererApi> p_renderer_api_;
};
}// namespace aria
