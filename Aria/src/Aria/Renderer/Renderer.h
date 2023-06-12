#pragma once

#include "Aria/Renderer/RendererApi.h"
#include "Camera.h"
#include "RenderCommand.h"
#include "RendererApi.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace aria {
class Renderer {
 public:
  static Scope<RendererApi> Init();
  static void BeginScene(const OrthographicCamera &camera);
  static void EndScene();
  static void Submit(const Ref<Shader> &shader, const Ref<VertexArray> &p_vertex_array,
                     const glm::mat4 transform = glm::mat4(1.0f));

  inline static RendererApi::Api GetApi() { return RendererApi::GetApi(); }
  static Scope<RendererApi> &GetApiInstance() { return p_renderer_api_; }

 private:
  struct SceneData {
    glm::mat4 vp_matrix;
  };
  static SceneData *p_scene_data_;

  static Scope<RendererApi> p_renderer_api_;
};
}// namespace aria
