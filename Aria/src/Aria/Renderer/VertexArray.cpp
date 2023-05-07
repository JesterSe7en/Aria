#include "ariapch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "VertexArray.h"
#include "RendererAPI.h"

namespace ARIA {

Ref<VertexArray> VertexArray::create() {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for vertex array generation")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLVertexArray>();
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(false, "API selected for vertex array generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API") return nullptr;
      return nullptr;
  }
}
}  // namespace ARIA
