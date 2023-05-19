#pragma once

#include <glm/glm.hpp>

#include "Aria/Renderer/VertexArray.h"

namespace aria {
class RendererApi {
 public:
  enum class Api {
    // DirectX and Vulkan would be added later
    NONE = 0,
    OPEN_GL = 1,
    DIRECT_X = 2,
    VULKAN = 3,
  };

 public:
  virtual void Init() = 0;
  virtual void Clear() = 0;
  virtual void SetClearColor(const glm::vec4 color) = 0;
  virtual void DrawIndexed(const Ref<VertexArray>& array) = 0;

  static Api GetApi() { return api_; }
  static Api Create();

 private:
  static Api api_;
};
}  // namespace ARIA