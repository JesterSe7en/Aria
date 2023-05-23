#include "ariapch.h"

#include "Platform/OpenGL/OpenGlVertexArray.hpp"

#include "VertexArray.hpp"
#include "RendererApi.hpp"

namespace aria {

Ref<VertexArray> VertexArray::Create() {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::NONE: ARIA_CORE_ASSERT(false, "No renderer API selected for vertex array generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:
      return std::make_shared<OpenGlVertexArray>();
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN: ARIA_CORE_ASSERT(false,
                                                    "API selected for vertex array generation is not implemented")
      return nullptr;
    default: ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
      return nullptr;
  }
}
}  // namespace ARIA
