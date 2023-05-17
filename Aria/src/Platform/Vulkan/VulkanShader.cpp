#include "VulkanShader.h"

#include "Aria/Core/Log.h"
#include "Platform/Vulkan/VulkanShader.h"

#include <stdint.h>
#include <vcruntime.h>

#include <filesystem>

namespace ARIA {
// Only accepts SPIR-V byte code...for now
VulkanShader::VulkanShader(const std::string& file_path) {
  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader bytecode file: {0}", file_path);
    return;
  }
  mRendererID = compile_shader(file_path);
}

uint32_t VulkanShader::compile_shader(const std::string& file_path) { auto bytecode = parse_bytecode_file(file_path); }

std::vector<char> VulkanShader::parse_bytecode_file(const std::string& file_path) const {
  std::ifstream bytecode_file(file_path, std::ios::ate | std::ios::binary);

  if (!bytecode_file.is_open()) {
    ARIA_CORE_ERROR("Cannot open shader bytecode file: {0}", file_path)
  }

  auto file_size = (size_t)bytecode_file.tellg();
  std::vector<char> buffer(file_size);

  bytecode_file.seekg(0);
  bytecode_file.read(buffer.data(), file_size);

  bytecode_file.close();
  return buffer;
}

void VulkanShader::bind() const {}

void VulkanShader::unbind() const {}
}  // namespace ARIA