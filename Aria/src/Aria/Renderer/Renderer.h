#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"

namespace ARIA {
class Renderer {
 public:
  static void BeginScene();
  static void EndScene();
  static void Submit(const std::shared_ptr<VertexArray>& vertex_array);

  inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }
};
}  // namespace ARIA