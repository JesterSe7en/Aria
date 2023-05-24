#include "ariapch.h"
#include "Renderer.hpp"
#include "Aria/Renderer/RenderCommand.hpp"
#include "Aria/Renderer/RendererApi.hpp"
#include "Platform/OpenGL/OpenGlShader.hpp"
#include "Shader.hpp"

namespace aria {

Renderer::SceneData *Renderer::p_scene_data_ = new Renderer::SceneData;

void Renderer::Init() { RenderCommand::Init(RendererApi::GetApi()); }

void Renderer::BeginScene(const OrthographicCamera &camera) { p_scene_data_->vp_matrix = camera.GetVpMatrix(); }
void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &p_vertex_array, const glm::mat4 transform) {
  std::dynamic_pointer_cast<OpenGlShader>(shader)->Bind();
  std::dynamic_pointer_cast<OpenGlShader>(shader)->SetUniformMat4F("u_ViewProjection", p_scene_data_->vp_matrix);
  std::dynamic_pointer_cast<OpenGlShader>(shader)->SetUniformMat4F("u_Transform", transform);
  p_vertex_array->Bind();
  RenderCommand::DrawIndexed(p_vertex_array);
}

void Renderer::CreatePipeline() { RenderCommand::CreatePipeline(); }
}// namespace aria
