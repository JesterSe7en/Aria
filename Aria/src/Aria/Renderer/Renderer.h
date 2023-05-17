#pragma once

#include "Aria/Renderer/RendererAPI.h"
#include "Camera.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace ARIA {
class Renderer {
 public:
  static void init();
  static void begin_scene(const OrthographicCamera& camera);
  static void end_scene();
  static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array,
                     const glm::mat4 transform = glm::mat4(1.0f));

  inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }
  static void create_pipeline();  // This is a hack for now

 private:
  struct SceneData {
    glm::mat4 mVPMatrix;
  };
  static SceneData* sSceneData;
};
}  // namespace ARIA
