#include "ariapch.h"
#include "VulkanShader.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/Shader.h"
#include "VulkanError.h"
#include "VulkanLib.h"
#include "VulkanRendererApi.h"
#include <filesystem>


namespace aria {

// Only accepts SPIR-V byte code...for now
VulkanShader::VulkanShader(const std::string &file_path) : shader_type_(ShaderType::NONE) {
  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader bytecode file: {0}", file_path)
    return;
  }

  CreateShader(file_path);
  AddToPipeline();
}

VulkanShader::VulkanShader(const std::string &file_path, ShaderType type) : shader_type_(type) {
  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader bytecode file: {0}", file_path)
    return;
  }

  name_ = std::filesystem::path(file_path).filename().stem().string();
  CreateShader(file_path);
  AddToPipeline();
}

VulkanShader::~VulkanShader() {
  auto vklib = VulkanLib::GetInstance();
  Ref<VulkanDeviceManager> device_manager = VulkanRendererApi::GetInstance().GetVulkanDeviceManager();
  vklib.ptr_vk_destroy_shader_module(device_manager->GetLogicalDevice(), vk_shader_module_, nullptr);
}

void VulkanShader::Bind() const {}

void VulkanShader::Unbind() const {}

void VulkanShader::CreateShader(const std::string &file_path) {
  auto bytecode = ParseByteCodeFile(file_path);
  CreateShaderModule(bytecode);
}

std::vector<char> VulkanShader::ParseByteCodeFile(const std::string &file_path) const {
  std::ifstream bytecode_file(file_path, std::ios::ate | std::ios::binary);

  if (!bytecode_file.is_open()) { ARIA_CORE_ERROR("Cannot open shader bytecode file: {0}", file_path) }

  auto file_size = bytecode_file.tellg();
  std::vector<char> buffer(file_size);

  bytecode_file.seekg(0);
  bytecode_file.read(buffer.data(), file_size);

  bytecode_file.close();
  return buffer;
}

void VulkanShader::CreateShaderModule(const std::vector<char> &code) {
  auto vklib = VulkanLib::GetInstance();
  VkShaderModuleCreateInfo create_info;
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.pNext = nullptr;
  create_info.flags = 0;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t *>(code.data());// TODO: use dynamic cast?

  Ref<VulkanDeviceManager> device_manager = VulkanRendererApi::GetInstance().GetVulkanDeviceManager();
  VkResult result =
      vklib.ptr_vk_create_shader_module(device_manager->GetLogicalDevice(), &create_info, nullptr, &vk_shader_module_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Cannot create shader module for {0}", name_)
}

void VulkanShader::AddToPipeline() { VulkanRendererApi::GetInstance().AddToPipeline(vk_shader_module_, shader_type_); }

}// namespace aria
