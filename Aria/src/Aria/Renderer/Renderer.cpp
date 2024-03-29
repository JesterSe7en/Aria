#include "ariapch.h"
#include "Renderer.h"
#include "Aria/Renderer/RenderCommand.h"
#include "Aria/Renderer/RendererApi.h"
#include "Platform/OpenGL/OpenGlShader.h"
#include "Shader.h"

namespace aria {

Renderer::SceneData *Renderer::p_scene_data_ = new Renderer::SceneData;
Scope<RendererApi> Renderer::p_renderer_api_ = nullptr;

Scope<RendererApi> Renderer::Init() {
  p_renderer_api_ = RenderCommand::Init(RendererApi::GetApi());
  return std::move(p_renderer_api_);
}

void Renderer::BeginScene(const OrthographicCamera &camera) { p_scene_data_->vp_matrix = camera.GetVpMatrix(); }
void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &p_vertex_array, const glm::mat4 transform) {
  std::dynamic_pointer_cast<OpenGlShader>(shader)->Bind();
  std::dynamic_pointer_cast<OpenGlShader>(shader)->SetUniformMat4F("u_ViewProjection", p_scene_data_->vp_matrix);
  std::dynamic_pointer_cast<OpenGlShader>(shader)->SetUniformMat4F("u_Transform", transform);
  p_vertex_array->Bind();
  RenderCommand::DrawIndexed(p_vertex_array);
}

}// namespace aria
