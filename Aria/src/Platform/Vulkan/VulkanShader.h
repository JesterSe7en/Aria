#pragma once

#include "Aria/Renderer/Shader.h"

#include <stdint.h>

namespace ARIA {
class VulkanShader : public Shader {
 public:
  explicit VulkanShader(const std::string& file_path);
  ~VulkanShader() override = default;  // TODO: this needs to properly destroy vulkan shader...

  // TBD, need to see how vulkan binds/unbinds shaders
  void bind() const override;
  void unbind() const override;

  const std::string& get_name() const override { return mName; }

 private:
  uint32_t mRendererID = 0;  // not sure if i need this...
  std::string mName;

  uint32_t compile_shader(const std::string& file_path);
  std::vector<char> parse_bytecode_file(const std::string& file_path) const;
};
}  // namespace ARIA