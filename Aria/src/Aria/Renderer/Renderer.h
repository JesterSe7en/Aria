#pragma once

#include "RendererAPI.h"

namespace ARIA {
class Renderer {
 public:
  static void BeginScene();
  static void EndScene();
  static void Submit(const std::shared_ptr<VertexArray>& vertex_array);

  inline static RendererAPI::API get_api() { return sRendererAPI; }

 private:
  static RendererAPI::API sRendererAPI;
};
}  // namespace ARIA