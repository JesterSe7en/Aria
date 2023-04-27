#include "ariapch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace ARIA {
void Renderer::BeginScene() {}
void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertex_array) {
  RenderCommand::DrawIndexed(vertex_array);
}
}  // namespace ARIA