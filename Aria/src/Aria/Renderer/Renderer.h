#pragma once

#include <glm/glm.hpp>

#include "RendererAPI.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace ARIA {
class Renderer {
 public:
  static void begin_scene(const OrthographicCamera& camera);
  static void end_scene();
  static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array,
                     const glm::mat4 transform = glm::mat4(1.0f));

  inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }

 private:
  struct SceneData {
    glm::mat4 mVPMatrix;
  };
  static SceneData* sSceneData;
};
}  // namespace ARIA