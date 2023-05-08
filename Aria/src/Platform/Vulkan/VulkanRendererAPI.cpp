#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Aria/Core/Log.h"
#include "ariapch.h"

#include "Aria/Renderer/VertexArray.h"

#include "VulkanRendererAPI.h"

#include <glad/gl.h>

namespace ARIA {
void VulkanRendererAPI::init() {
  ARIA_CORE_TRACE("hi from vulkan renderer api")
  glad_glEnable(GL_BLEND);
  glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void VulkanRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT); }
void VulkanRendererAPI::set_clear_color(const glm::vec4 color) { glClearColor(color.r, color.g, color.b, color.a); }
void VulkanRendererAPI::draw_indexed(const Ref<VertexArray>& vertex_array) {
  glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
}
}  // namespace ARIA