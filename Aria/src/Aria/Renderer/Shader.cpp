#include "ariapch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Shader.h"
#include "Renderer.h"

namespace ARIA {

Shader *Shader::Create(const std::string &vertex_src, const std::string &fragment_src) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(vertex_src, fragment_src);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented");
    default:
      ARIA_CORE_ASSERT("Unknown API");
      return nullptr;
  }
}

Shader *Shader::Create(const std::string &file_path) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(file_path);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented");
    default:
      ARIA_CORE_ASSERT("Unknown API");
      return nullptr;
  }
}

}  // namespace ARIA
