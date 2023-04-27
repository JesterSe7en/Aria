#include "ariapch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace ARIA {

void Renderer::begin_scene() {}
void Renderer::end_scene() {}

void Renderer::submit(const std::shared_ptr<VertexArray>& vertex_array) {
  RenderCommand::draw_indexed(vertex_array);
}
}  // namespace ARIA