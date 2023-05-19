#include <memory>
#include <string_view>
#include "Aria/Core/Base.h"
#include "ariapch.h"

#include "Platform/OpenGL/OpenGlShader.h"

#include "Shader.h"
#include "Renderer.h"

namespace aria {

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::NONE:
      ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:
      return std::make_shared<OpenGlShader>(name, vertex_src, fragment_src);
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN:
      ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::string& file_path) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::NONE:
      ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:
      return std::make_shared<OpenGlShader>(file_path);
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN:
      ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
  auto& kName = shader->GetName();
  ARIA_CORE_ASSERT(!Exists(kName), "Shader already exists")
  shaders_[kName] = shader;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
  ARIA_CORE_ASSERT(!Exists(name), "Shader already exists")
  shaders_[name] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& file_path) {
  auto shader = Shader::Create(file_path);
  Add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& file_path) {
  auto shader = Shader::Create(file_path);
  Add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) {
  ARIA_CORE_ASSERT(Exists(name), "Shader not found")
  return shaders_[name];
}

bool ShaderLibrary::Exists(const std::string& name) { return shaders_.find(name) != shaders_.end(); }

}  // namespace ARIA
