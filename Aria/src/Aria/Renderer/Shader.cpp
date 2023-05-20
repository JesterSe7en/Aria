#include "ariapch.h"

#include "Shader.h"

#include "Aria/Core/Base.h"
#include "Aria/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"
#include "Platform/Vulkan/VulkanShader.h"

#include <memory>
#include <string_view>

namespace aria {

Ref<Shader> Shader::Create(const std::string &name, const std::string &vertex_src, const std::string &fragment_src) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::NONE: ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:return std::make_shared<OpenGlShader>(name, vertex_src, fragment_src);
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN: ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented")
      return nullptr;
    default: ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::string &file_path) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::NONE: ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:return std::make_shared<OpenGlShader>(file_path);
    case RendererApi::Api::DIRECT_X: ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented")
      return nullptr;
    case RendererApi::Api::VULKAN:return std::make_shared<VulkanShader>(file_path);
    default: ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::string &file_path, ShaderType type) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::VULKAN:return std::make_shared<VulkanShader>(file_path, type);
    default: ARIA_CORE_ASSERT(false, "Should not be calling this other than for vulkan... for now")
      return nullptr;
  }
}

void ShaderLibrary::Add(const Ref<Shader> &shader) {
  auto &name = shader->GetName();
  ARIA_CORE_ASSERT(!Exists(name), "Shader already exists")
  shaders_[name] = shader;
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
  ARIA_CORE_ASSERT(!Exists(name), "Shader already exists")
  shaders_[name] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &file_path) {
  auto shader = Shader::Create(file_path);
  Add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &file_path, ShaderType type) {
  auto shader = Shader::Create(file_path, type);
  Add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &file_path) {
  auto shader = Shader::Create(file_path);
  Add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string &name) {
  ARIA_CORE_ASSERT(Exists(name), "Shader not found")
  return shaders_[name];
}

bool ShaderLibrary::Exists(const std::string &name) { return shaders_.find(name) != shaders_.end(); }

}  // namespace ARIA
