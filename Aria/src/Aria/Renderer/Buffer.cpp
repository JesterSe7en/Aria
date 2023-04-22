#include "ariapch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Aria/Log.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Aria {

VertexBuffer* VertexBuffer::Create(float* buffer, uint32_t size) {
  RendererAPI api = Renderer::get_API();
  switch (api) {
    case RendererAPI::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for vertex buffer generation")
      return nullptr;
    case RendererAPI::OpenGL:
      return OpenGLVertexBuffer::Create(buffer, size);
      break;
    case RendererAPI::DirectX:
    case RendererAPI::Vulkan:
      ARIA_CORE_ASSERT(
          false,
          "{0} API selected for vertex buffer generation is not implemeneted",
          api);
      return nullptr;
    default:
      ARIA_CORE_WARN("Unknown API", api);
      return nullptr;
  }
}

IndexBuffer* IndexBuffer::Create(float* buffer, uint32_t size) {
  return nullptr;
}

}  // namespace Aria