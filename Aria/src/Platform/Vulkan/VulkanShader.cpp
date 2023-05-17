#include "VulkanShader.h"

#include "Aria/Core/Log.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
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
  create_shader(file_path);
}

VulkanShader::~VulkanShader() { vkDestroyShaderModule(VulkanRendererAPI::get_vk_device(), mShaderModule, nullptr); }

void VulkanShader::bind() const {}

void VulkanShader::unbind() const {}

void VulkanShader::create_shader(const std::string& file_path) {
  auto bytecode = parse_bytecode_file(file_path);
  create_shader_module(bytecode);
}

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

void VulkanShader::create_shader_module(const std::vector<char>& code) {
  VkShaderModuleCreateInfo create_info;
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.pNext = nullptr;
  create_info.flags = 0;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());  // TODO: use dynamic cast?

  if (vkCreateShaderModule(VulkanRendererAPI::get_vk_device(), &create_info, nullptr, &mShaderModule) != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create shader module for {0}", mName)  // TODO: more useful if file path?
  }
}

}  // namespace ARIA