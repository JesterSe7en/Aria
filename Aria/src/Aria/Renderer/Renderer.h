#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"
#include "RenderCommand.h"

namespace ARIA {
class Renderer {
 public:
  static void begin_scene();
  static void end_scene();
  static void submit(const std::shared_ptr<VertexArray>& vertex_array);

  inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }
};
}  // namespace ARIA