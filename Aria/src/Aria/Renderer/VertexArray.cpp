#include "ariapch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ARIA {

Ref<VertexArray> VertexArray::create() {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for vertex array generation")
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLVertexArray>();
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
