#pragma once

#include <glm/glm.hpp>

#include "RendererApi.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace aria {
class Renderer {
 public:
  static void Init();
  static void BeginScene(const OrthographicCamera& camera);
  static void EndScene();
  static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array,
                     const glm::mat4 transform = glm::mat4(1.0f));

  inline static RendererApi::Api GetApi() { return RendererApi::GetApi(); }

 private:
  struct SceneData {
    glm::mat4 vp_matrix;
  };
  static SceneData* p_scene_data_;
};
}  // namespace ARIA