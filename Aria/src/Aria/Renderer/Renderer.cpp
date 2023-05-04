#include "ariapch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace ARIA {

Renderer::SceneData* Renderer::sSceneData = new Renderer::SceneData;

void Renderer::begin_scene(const OrthographicCamera& camera) { sSceneData->mVPMatrix = camera.get_vp_matrix(); }
void Renderer::end_scene() {}

void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array,
                      const glm::mat4 transform) {
  shader->bind();
  shader->set_uniform_mat4f("u_ViewProjection", sSceneData->mVPMatrix);
  shader->set_uniform_mat4f("u_Transform", transform);
  vertex_array->bind();
  RenderCommand::draw_indexed(vertex_array);
}
}  // namespace ARIA
