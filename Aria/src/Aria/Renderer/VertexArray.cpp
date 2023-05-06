#include "ariapch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "VertexArray.h"
#include "RendererAPI.h"

namespace ARIA {

VertexArray* VertexArray::create() {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for vertex array generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexArray();
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(false, "API selected for vertex array generation is not implemented");
    default:
      ARIA_CORE_ASSERT("Unknown API");
      return nullptr;
  }
}
}  // namespace ARIA
