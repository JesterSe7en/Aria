#include "ariapch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Aria/Log.h"
#include "Aria/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ARIA {

VertexArray* VertexArray::create() {
  Renderer::API api = Renderer::get_api();
  switch (api) {
    case Renderer::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for vertex array generation")
    case Renderer::API::OpenGL:
      return new OpenGLVertexArray();
      break;
    case Renderer::API::DirectX:
    case Renderer::API::Vulkan:
      ARIA_CORE_ASSERT(
          false,
          "API selected for vertex array generation is not implemented", );
    default:
      ARIA_CORE_ASSERT("Unknown API");
      return nullptr;
  }
}
}  // namespace ARIA
