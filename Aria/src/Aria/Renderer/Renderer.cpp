#include "ariapch.h"

#include "Platform/OpenGL/OpenGlShader.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace aria {

Renderer::SceneData* Renderer::p_scene_data_ = new Renderer::SceneData;

void Renderer::Init() {
  RenderCommand::Init();
}

void Renderer::BeginScene(const OrthographicCamera& camera) { p_scene_data_->vp_matrix = camera.GetVpMatrix(); }
void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array,
                      const glm::mat4 transform) {
  std::dynamic_pointer_cast<OpenGlShader>(shader)->Bind();
  std::dynamic_pointer_cast<OpenGlShader>(shader)->SetUniformMat4F("u_ViewProjection", p_scene_data_->vp_matrix);
  std::dynamic_pointer_cast<OpenGlShader>(shader)->SetUniformMat4F("u_Transform", transform);
  vertex_array->Bind();
  RenderCommand::DrawIndexed(vertex_array);
}
}  // namespace ARIA
