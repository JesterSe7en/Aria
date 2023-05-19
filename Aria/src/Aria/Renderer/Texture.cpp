#include "ariapch.h"

#include "Platform/OpenGL/OpenGlTexture2D.h"

#include "Texture.h"
#include "RendererApi.h"

namespace aria {
// OpenGL requires textures to be loaded from bottom left to top right
Ref<Texture2D> Texture2D::Create(const std::string& path) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::NONE:
      ARIA_CORE_ASSERT(false, "No renderer API selected for texture generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:
      return std::make_shared<OpenGlTexture2D>(path);
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN:
      ARIA_CORE_ASSERT(false, "API selected for texture generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}
}  // namespace ARIA