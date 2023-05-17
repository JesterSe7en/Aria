#include "ariapch.h"

#include "Shader.h"

#include "Aria/Core/Base.h"
#include "Aria/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Renderer.h"

#include <memory>
#include <string_view>

namespace ARIA {

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLShader>(name, vertex_src, fragment_src);
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::string& file_path) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false, "No renderer API selected for shader generation")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLShader>(file_path);
    case RendererAPI::API::DirectX:
      ARIA_CORE_ASSERT(false, "API selected for shader generation is not implemented")
      return nullptr;
    case RendererAPI::API::Vulkan:
      return std::make_shared<VulkanShader>(file_path);
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::string& file_path, ShaderType type) {
  switch (RendererAPI::get_api()) {
    case RendererAPI::API::Vulkan:
      return std::make_shared<VulkanShader>(file_path, type);
    default:
      ARIA_CORE_ASSERT(false, "Should not be calling this other than for vulkan... for now")
      return nullptr;
  }
}

void ShaderLibrary::add(const Ref<Shader>& shader) {
  auto& name = shader->get_name();
  ARIA_CORE_ASSERT(!exists(name), "Shader already exists")
  mShaders[name] = shader;
}

void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
  ARIA_CORE_ASSERT(!exists(name), "Shader already exists")
  mShaders[name] = shader;
}

Ref<Shader> ShaderLibrary::load(const std::string& file_path) {
  auto shader = Shader::Create(file_path);
  add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::load(const std::string& file_path, ShaderType type) {
  auto shader = Shader::Create(file_path, type);
  add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& file_path) {
  auto shader = Shader::Create(file_path);
  add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::get(const std::string& name) {
  ARIA_CORE_ASSERT(exists(name), "Shader not found")
  return mShaders[name];
}

bool ShaderLibrary::exists(const std::string& name) { return mShaders.find(name) != mShaders.end(); }

}  // namespace ARIA
