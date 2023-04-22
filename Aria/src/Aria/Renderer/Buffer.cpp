#include "ariapch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Aria/Log.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Aria {

VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size) {
  RendererAPI::API api = RendererAPI::GetAPI();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for vertex buffer generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexBuffer(verticies, size);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false,
          "API selected for vertex buffer generation is not implemented", );
    default:
      ARIA_CORE_ASSERT("Unknown API");
  }
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
  RendererAPI::API api = RendererAPI::GetAPI();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for index buffer generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLIndexBuffer(indices, count);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false, "API selected for index buffer generation is not implemented");
    default:
      ARIA_CORE_ASSERT("Unknown API");
  }
}
}  // namespace Aria