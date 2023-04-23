#include "ariapch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Aria/Log.h"
#include "Aria/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Aria {

VertexArray* VertexArray::Create() {
  RendererAPI::API api = RendererAPI::GetAPI();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for vertex array generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexArray();
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false,
          "API selected for vertex array generation is not implemented", );
    default:
      ARIA_CORE_ASSERT("Unknown API");
  }
}
}  // namespace Aria
