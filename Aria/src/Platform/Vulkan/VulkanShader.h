#pragma once

#include "Aria/Renderer/Shader.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>

namespace aria {
class VulkanShader : public Shader {
 public:
  explicit VulkanShader(const std::string &file_path);
  VulkanShader(const std::string &file_path, ShaderType type);
  ~VulkanShader() override;

  // TBD, need to see how vulkan binds/unbinds shaders
  void Bind() const override;
  void Unbind() const override;

  const std::string &GetName() const override { return name_; }

 private:
  VkShaderModule vk_shader_module_ = VK_NULL_HANDLE;
  ShaderType shader_type_;
  std::string name_;

  void CreateShader(const std::string &file_path);
  std::vector<char> ParseByteCodeFile(const std::string &file_path) const;
  void CreateShaderModule(const std::vector<char> &code);
  void AddToPipeline();
};
}// namespace aria
