#include "ariapch.h"

#include "Aria/Renderer/VertexArray.h"

#include "OpenGlRendererApi.h"

#include <glad/gl.h>

namespace aria {
void OpenGlRendererApi::Init() {
  glad_glEnable(GL_BLEND);
  glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void OpenGlRendererApi::Clear() { glClear(GL_COLOR_BUFFER_BIT); }
void OpenGlRendererApi::SetClearColor(const glm::vec4 color) { glClearColor(color.r, color.g, color.b, color.a); }
void OpenGlRendererApi::DrawIndexed(const Ref<VertexArray> &p_vertex_array) {
  glDrawElements(GL_TRIANGLES, p_vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
}  // namespace ARIA