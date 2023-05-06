#include "ariapch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace ARIA {

Renderer::SceneData* Renderer::sSceneData = new Renderer::SceneData;

void Renderer::init() {
  RenderCommand::init();
}

void Renderer::begin_scene(const OrthographicCamera& camera) { sSceneData->mVPMatrix = camera.get_vp_matrix(); }
void Renderer::end_scene() {}

void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array,
                      const glm::mat4 transform) {
  std::dynamic_pointer_cast<OpenGLShader>(shader)->bind();
  std::dynamic_pointer_cast<OpenGLShader>(shader)->set_uniform_mat4f("u_ViewProjection", sSceneData->mVPMatrix);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->set_uniform_mat4f("u_Transform", transform);
  vertex_array->bind();
  RenderCommand::draw_indexed(vertex_array);
}
}  // namespace ARIA
