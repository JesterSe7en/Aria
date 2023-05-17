#pragma once

#include "Aria/Renderer/Shader.h"
#include "vulkan/vulkan_core.h"

#include <cstdint>

namespace ARIA {
class VulkanShader : public Shader {
 public:
  explicit VulkanShader(const std::string& file_path);
  VulkanShader(const std::string& file_path, ShaderType type);
  ~VulkanShader() override;  // TODO: this needs to properly destroy vulkan shader...

  // TBD, need to see how vulkan binds/unbinds shaders
  void bind() const override;
  void unbind() const override;

  const std::string& get_name() const override { return mName; }

 private:
  VkShaderModule mShaderModule;
  ShaderType mType;
  std::string mName;

  void create_shader(const std::string& file_path);
  std::vector<char> parse_bytecode_file(const std::string& file_path) const;
  void create_shader_module(const std::vector<char>& code);
  void add_to_pipeline();
};
}  // namespace ARIA