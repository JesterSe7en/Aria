#include "ariapch.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

#include "Texture.h"
#include "RendererAPI.h"

namespace ARIA {
// OpenGL requires textures to be loaded from bottom left to top right
Ref<Texture2D> Texture2D::create(const std::string& path) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for texture generation")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(false, "API selected for texture generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}
}  // namespace ARIA